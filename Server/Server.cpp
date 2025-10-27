#include "pch.h"
//#include <iostream>
//#include <thread>
//#include <vector>
//using namespace std;
//#include <atomic>
//#include <mutex>

// 서버
// 1) 새로운 소켓 생성 (socket)
// 2) 소켓에 주소/포트 번호 설정 (bind)
// 3) 소켓 일 시키기 (listen)
// 4) 손님 접속 (accept)
// 5) 클라와 통신

int main()
{
	cout << "=============== Server Program ===============" << endl;
	// 초기화
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	// 1) 소켓 생성
	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
	// int32 errorCode = ::WSAGetLastError();
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// 2) 주소/포트 번호 설정 (bind)
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonf(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if(::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	// 3) 업무 개시 (listen)
	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	// 4)
	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		// 클라이언트 소켓 받기
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;

		char ip[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		cout << "Client Connected! IP = " << ip << endl;

		while (true)
		{
			// 클라에서 받기
			char recvBuffer[100];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
				return 0;

			cout << "Recv Data : " << recvBuffer << endl;
			cout << "Recv Data Len : " << recvLen << endl;

			// 클라로 보내기
			int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
			if (resultCode == SOCKET_ERROR)
				return 0;

		}
	}

	::closesocket(listenSocket);
	::WSACleanup();
}