#include "pch.h"
#include "NetMgr.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"

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
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); }, // TODO : SessionManager µî
		1);

	assert(_service->Start());

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
}

ServerSessionRef NetMgr::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetMgr::SendPacket(SendBufferRef sendBuffer)
{
	if (_session)
		_session->Send(sendBuffer);
}
