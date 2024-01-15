#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,
	C_LeaveGame = 3,

	S_MyPlayer = 4,
	S_AddObject = 5,
	S_RemoveObject = 6,

	C_RemoveObject = 7,
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

	S_Gold = 36,
};
class Creature;
class ClientPacketHandler
{
public:
	static void HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len);

	// 받기
	static void Handle_S_TEST(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Fire(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_SendMessage(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Teleport(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Gold(ServerSessionRef session, BYTE* buffer, int32 len);

	// 보내기
	static SendBufferRef Make_C_Move();

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

	static SendBufferRef Make_C_Fire(uint64 ownerid);
	static SendBufferRef Make_C_Hit(Protocol::ObjectInfo& objectInfo, uint64 attackerId);
	static SendBufferRef Make_C_SendMessage(uint64 objectId, time_t time, string str);
	static SendBufferRef Make_C_RemoveObject(uint64 objectId);
	static SendBufferRef Make_C_Revive(Protocol::ObjectInfo& objectInfo);
};

