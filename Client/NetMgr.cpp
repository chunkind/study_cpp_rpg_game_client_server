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
		NetAddress(L"127.0.0.1", 8989),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); }, // TODO : SessionManager 등
		1);

	// [Release 모드 버그 수정]
	// Release 모드에서는 assert()가 제거되므로, assert 안에 있던 Start() 함수가 실행되지 않음
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
