#include "pch.h"
#include <iostream>
//#include <WinSock2.h>
#include <chrono>
//#include <thread>

//using namespace std;
using namespace std::chrono_literals; // C++14 이상에서 1s 리터럴 사용 가능

// 클라
// 1) 소켓 생성
// 2) 서버에 연결 요청
// 3) 통신
int main()
{
	cout << "=============== Client Program ===============" << endl;
	// 초기화
    WSADATA wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

	// 1) 소켓 생성
	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
	// int32 errorCode = ::WSAGetLastError();
    SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
        return 0;

	// 2) 주소/포트 번호 설정 (bind)
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::htonf(INADDR_ANY);
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// 3) 연결
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	// ---------
	// 연결 성공!
	cout << "Connected To Server!" << endl;

	while (true)
	{
		// 서버로 보내기
		char sendBuffer[100] = "Hello ! I am Client!";
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
			return 0;

		// 서버에서 받기
		char recvBuffer[100];
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
			return 0;

		cout << "Echo Data : " << recvBuffer << endl;

		std::this_thread::sleep_for(1s); // 또는 std::chrono::seconds(1)
	}

	::closesocket(clientSocket);
    ::WSACleanup();
}