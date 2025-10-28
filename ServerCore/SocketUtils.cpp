#include "pch.h"
#include "SocketUtils.h"

/*----------------------------------------------------------------
 ******* SocketUtils *********************************************

함수명						기능 요약
Init()						Winsock 초기화 및 확장 함수 포인터 로드
Clear()						Winsock 종료
BindWindowsFunction()		WSA 확장 함수 포인터 바인딩
CreateSocket()				비동기용 TCP 소켓 생성
SetLinger()					연결 종료 시 대기 설정
SetReuseAddress()			포트 재사용 옵션 설정
SetRecv/SendBufferSize()	송수신 버퍼 크기 설정
SetTcpNoDelay()				Nagle 알고리즘 비활성화
SetUpdateAcceptSocket()		AcceptEx로 생성된 소켓 초기화
Bind() / BindAnyAddress()	주소 바인딩
Listen()					수신 대기 상태로 전환
Close()						소켓 안전 종료

----------------------------------------------------------------*/

// Winsock 확장 함수 포인터 선언 (런타임에 동적 로드)
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

/*
 * @brief 소켓 유틸리티 초기화 함수
 * - WSAStartup()을 통해 WinSock2 라이브러리 초기화
 * - ConnectEx / DisconnectEx / AcceptEx 같은 확장 함수 주소를 런타임에 로드
 */
void SocketUtils::Init()
{
	WSADATA wsaData;
	assert(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

	// 더미 소켓 생성 (확장 함수 포인터를 얻기 위해 필요)
	SOCKET dummySocket = CreateSocket();

	// 각 확장 함수 포인터를 WSAIoctl로 획득
	assert(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

	// 더미 소켓 닫기
	Close(dummySocket);
}

/*
 * @brief Winsock 라이브러리 해제 함수
 * - 프로그램 종료 시 반드시 호출해야 함
 */
void SocketUtils::Clear()
{
	::WSACleanup();
}

/*
 * @brief WSA 확장 함수 포인터를 런타임에 바인딩
 * @param socket  임시 소켓 핸들
 * @param guid    요청할 확장 함수의 GUID (예: WSAID_CONNECTEX)
 * @param fn      함수 포인터를 저장할 주소
 * @return 성공 시 true, 실패 시 false
 */
bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(
		socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid, sizeof(guid),
		fn, sizeof(*fn),
		OUT & bytes,
		NULL, NULL);
}

/*
 * @brief Overlapped I/O용 TCP 소켓 생성
 * @return WSASocket 핸들 반환
 */
SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

/*
 * @brief 소켓의 LINGER 옵션 설정 (종료 시 대기 여부)
 * @param onoff  1이면 linger 동작 활성화
 * @param linger 대기 시간(초)
 */
bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

/*
 * @brief 포트 재사용 옵션 설정 (서버 재시작 시 유용)
 */
bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

/*
 * @brief 수신 버퍼 크기 설정
 */
bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

/*
 * @brief 송신 버퍼 크기 설정
 */
bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

/*
 * @brief Nagle 알고리즘 비활성화 (TCP 지연 최소화)
 * @param flag true면 Nagle 비활성화(TCP_NODELAY on)
 */
bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

/*
 * @brief Listen 소켓의 속성을 새로 Accept된 Client 소켓에 반영
 * - AcceptEx 사용 시 필수 설정
 */
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

/*
 * @brief 지정한 네트워크 주소(NetAddress)에 소켓 바인딩
 */
bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

/*
 * @brief 지정한 SOCKADDR_IN 구조체에 소켓 바인딩
 */
bool SocketUtils::Bind(SOCKET socket, SOCKADDR_IN sockAddr)
{
	return SOCKET_ERROR != ::bind(socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN));
}

/*
 * @brief 모든 IP(INADDR_ANY)에 지정 포트로 바인딩
 * - 서버에서 일반적으로 사용하는 방식
 */
bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));
}

/*
 * @brief 소켓을 수신 대기 상태(Listen)로 전환
 * @param backlog 대기 큐 크기 (최대 연결 대기 수)
 */
bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

/*
 * @brief 소켓을 안전하게 닫고 INVALID_SOCKET으로 초기화
 */
void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}