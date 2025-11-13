#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_MyPlayer = 4,
	S_AddObject = 5,

	C_RemoveObject = 6,
	S_RemoveObject = 7,
	
	C_Move = 10,
	S_Move = 11,

	C_Attack = 12,
	S_Attack = 13,
};

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(GameSessionRef session, BYTE* buffer, int32 len);

	// �ޱ�
	static void Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Attack(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_RemoveObject(GameSessionRef session, BYTE* buffer, int32 len);

	// ������
	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs);
	static SendBufferRef Make_S_EnterGame();
	static SendBufferRef Make_S_MyPlayer(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_AddObject(const Protocol::S_AddObject& pkt);
	static SendBufferRef Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt);
	static SendBufferRef Make_S_Move(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_Attack(const Protocol::ObjectInfo& info);


	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		// [Release 모드 버그 수정]
		// Release 모드에서는 assert()가 제거되므로, assert 안에 있던 SerializeToArray 함수가 실행되지 않아
		// 빈 패킷(모든 바이트가 0)이 전송되던 문제가 있었음
		// 따라서 함수 결과를 변수에 먼저 저장한 후, assert로 검증하는 방식으로 수정
		bool serializeResult = pkt.SerializeToArray(&header[1], dataSize);
		assert(serializeResult);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};