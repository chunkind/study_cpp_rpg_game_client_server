#include "pch.h"
#include "SocketUtils.h"

/*----------------------------------------------------------------
 ******* SocketUtils *********************************************

�Լ���						��� ���
Init()						Winsock �ʱ�ȭ �� Ȯ�� �Լ� ������ �ε�
Clear()						Winsock ����
BindWindowsFunction()		WSA Ȯ�� �Լ� ������ ���ε�
CreateSocket()				�񵿱�� TCP ���� ����
SetLinger()					���� ���� �� ��� ����
SetReuseAddress()			��Ʈ ���� �ɼ� ����
SetRecv/SendBufferSize()	�ۼ��� ���� ũ�� ����
SetTcpNoDelay()				Nagle �˰��� ��Ȱ��ȭ
SetUpdateAcceptSocket()		AcceptEx�� ������ ���� �ʱ�ȭ
Bind() / BindAnyAddress()	�ּ� ���ε�
Listen()					���� ��� ���·� ��ȯ
Close()						���� ���� ����

----------------------------------------------------------------*/

// Winsock Ȯ�� �Լ� ������ ���� (��Ÿ�ӿ� ���� �ε�)
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

/*
 * @brief ���� ��ƿ��Ƽ �ʱ�ȭ �Լ�
 * - WSAStartup()�� ���� WinSock2 ���̺귯�� �ʱ�ȭ
 * - ConnectEx / DisconnectEx / AcceptEx ���� Ȯ�� �Լ� �ּҸ� ��Ÿ�ӿ� �ε�
 */
void SocketUtils::Init()
{
	WSADATA wsaData;
	assert(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

	// ���� ���� ���� (Ȯ�� �Լ� �����͸� ��� ���� �ʿ�)
	SOCKET dummySocket = CreateSocket();

	// �� Ȯ�� �Լ� �����͸� WSAIoctl�� ȹ��
	assert(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

	// ���� ���� �ݱ�
	Close(dummySocket);
}

/*
 * @brief Winsock ���̺귯�� ���� �Լ�
 * - ���α׷� ���� �� �ݵ�� ȣ���ؾ� ��
 */
void SocketUtils::Clear()
{
	::WSACleanup();
}

/*
 * @brief WSA Ȯ�� �Լ� �����͸� ��Ÿ�ӿ� ���ε�
 * @param socket  �ӽ� ���� �ڵ�
 * @param guid    ��û�� Ȯ�� �Լ��� GUID (��: WSAID_CONNECTEX)
 * @param fn      �Լ� �����͸� ������ �ּ�
 * @return ���� �� true, ���� �� false
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
 * @brief Overlapped I/O�� TCP ���� ����
 * @return WSASocket �ڵ� ��ȯ
 */
SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

/*
 * @brief ������ LINGER �ɼ� ���� (���� �� ��� ����)
 * @param onoff  1�̸� linger ���� Ȱ��ȭ
 * @param linger ��� �ð�(��)
 */
bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

/*
 * @brief ��Ʈ ���� �ɼ� ���� (���� ����� �� ����)
 */
bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

/*
 * @brief ���� ���� ũ�� ����
 */
bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

/*
 * @brief �۽� ���� ũ�� ����
 */
bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

/*
 * @brief Nagle �˰��� ��Ȱ��ȭ (TCP ���� �ּ�ȭ)
 * @param flag true�� Nagle ��Ȱ��ȭ(TCP_NODELAY on)
 */
bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

/*
 * @brief Listen ������ �Ӽ��� ���� Accept�� Client ���Ͽ� �ݿ�
 * - AcceptEx ��� �� �ʼ� ����
 */
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

/*
 * @brief ������ ��Ʈ��ũ �ּ�(NetAddress)�� ���� ���ε�
 */
bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

/*
 * @brief ������ SOCKADDR_IN ����ü�� ���� ���ε�
 */
bool SocketUtils::Bind(SOCKET socket, SOCKADDR_IN sockAddr)
{
	return SOCKET_ERROR != ::bind(socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN));
}

/*
 * @brief ��� IP(INADDR_ANY)�� ���� ��Ʈ�� ���ε�
 * - �������� �Ϲ������� ����ϴ� ���
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
 * @brief ������ ���� ��� ����(Listen)�� ��ȯ
 * @param backlog ��� ť ũ�� (�ִ� ���� ��� ��)
 */
bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

/*
 * @brief ������ �����ϰ� �ݰ� INVALID_SOCKET���� �ʱ�ȭ
 */
void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}