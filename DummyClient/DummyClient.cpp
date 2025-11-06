#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "CoreUtil.h"

char sendData[] = "Hello World";

class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		cout << "Connected To Server" << endl;

		SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);
	}

	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		this_thread::sleep_for(0.1s);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);

		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected" << endl;
	}
};

void initDummyClient()
{
	this_thread::sleep_for(1s);

	SocketUtils::Init();

	ClientServiceRef service = make_shared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[]() { return make_shared<ServerSession>(); }, // TODO : SessionManager 등
		5);

	assert(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
	SocketUtils::Clear();
}

int main()
{
	//initDummyClient();

	// CoreUtil::GetRandom 테스트
	cout << "=== CoreUtil::GetRandom 테스트 ===" << endl;

	// 1부터 10 사이의 랜덤 숫자 10개 생성
	cout << "1~10 사이의 랜덤 숫자 10개:" << endl;
	for (int i = 0; i < 10; i++)
	{
		int32 randomValue = CoreUtil::GetRandom(1, 10);
		cout << "랜덤 값 " << i + 1 << ": " << randomValue << endl;
	}

	cout << endl;

	// 0부터 100 사이의 랜덤 숫자 5개 생성
	cout << "0~100 사이의 랜덤 숫자 5개:" << endl;
	for (int i = 0; i < 5; i++)
	{
		int32 randomValue = CoreUtil::GetRandom(0, 100);
		cout << "랜덤 값 " << i + 1 << ": " << randomValue << endl;
	}

	return 0;
}