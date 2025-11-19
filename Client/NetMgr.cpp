#include "pch.h"
#include "NetMgr.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "Core.h"

NetMgr::NetMgr()
{

}

NetMgr::~NetMgr()
{

}

void NetMgr::Init()
{
	SocketUtils::Init();

	_service = make_shared<ClientService>(
		NetAddress(L"192.168.45.148", 8989),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); },
		1);

	bool startResult = _service->Start();
	assert(startResult);

	/*for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}*/
}

void NetMgr::Update()
{
	_service->GetIocpCore()->Dispatch(0);

	// 패킷 서버로 전송
	SyncToServer();
}

void NetMgr::SyncToServer()
{
	if (!_session)
		return;

	if (_sendQueue.empty())
		return;

	while (!_sendQueue.empty())
	{
		SendBufferRef sendBuff = _sendQueue.front();
		_sendQueue.pop();

		_session->Send(sendBuff);
	}
}

ServerSessionRef NetMgr::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetMgr::SendPacket(SendBufferRef sendBuffer)
{
	if (sendBuffer != nullptr)
		_sendQueue.push(sendBuffer);
}
