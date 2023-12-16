#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_MyPlayer = 4,
	S_AddObject = 5,
	S_RemoveObject = 6,
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
	
	
	
	// 보내기
	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs);
	static SendBufferRef Make_S_EnterGame();
	static SendBufferRef Make_S_MyPlayer(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_AddObject(const Protocol::S_AddObject& pkt);
	static SendBufferRef Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt);

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