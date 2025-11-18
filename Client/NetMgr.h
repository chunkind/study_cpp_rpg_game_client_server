#pragma once

using ServerSessionRef = shared_ptr<class ServerSession>;

class NetMgr
{
	SING(NetMgr)

public:
	void Init();
	void Update();
	void SyncToServer();

	ServerSessionRef CreateSession();
	void SendPacket(SendBufferRef sendBuffer);

private:
	ClientServiceRef _service;
	ServerSessionRef _session;
	queue<SendBufferRef> _sendQueue;
};

