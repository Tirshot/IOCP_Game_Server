#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_MyPlayer = 4,
	S_AddObject = 5,
	S_RemoveObject = 6,

	// 해킹 방지를 위하여 번호를 비연속적으로 설정가능
	C_Move = 10,
	S_Move = 11,

	C_Fire = 20,
	S_Fire = 21,

	C_Hit = 22,
	S_Hit = 23,

	C_SendMessage = 30,
	S_SendMessage = 31,

	C_Revive = 32,
	C_Teleport = 33,
	S_Teleport = 34,
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

	// 받기
	static void Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Hit(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Fire(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_SendMessage(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Revive(GameSessionRef session, BYTE* buffer, int32 len);
	
	
	// 보내기
	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs);
	static SendBufferRef Make_S_EnterGame();
	static SendBufferRef Make_S_MyPlayer(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_AddObject(const Protocol::S_AddObject& pkt);
	static SendBufferRef Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt);
	static SendBufferRef Make_S_Move(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_SendMessage(uint64 objectId, uint64 time, string str);
	static SendBufferRef Make_S_Teleport(uint64 objectId, int32 cellPosX, int32 cellPosY);

	// Arrow의 info를 넘김
	static SendBufferRef Make_S_Fire(const Protocol::ObjectInfo& info, uint64 id);

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};