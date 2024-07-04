#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,
	C_LeaveGame = 3,

	S_MyPlayer = 4,
	S_AddObject = 5,
	S_RemoveObject = 6,

	// ��ŷ ������ ���Ͽ� ��ȣ�� �񿬼������� ��������
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
	C_SyncInventory = 35,

	C_Quest = 40,
	S_Quest = 41,

	C_QuestAccept = 42,
	S_QuestProcess = 43,
	S_QuestComplete = 44,
	C_QuestFinish = 45,

	C_QuestList = 46,
	S_QuestList = 47,
	S_QuestState = 48,	// ��Ȱ �� ���൵ ����

	C_Heal = 49,
	C_MPRecover = 50,
	S_MPRecover = 51,

	C_AddItem = 70,
	C_EquipItem = 71,
	S_AddItem = 72,
	S_EquipItem = 73,

	S_ItemDrop = 80,
	S_Gold = 81,

	S_Reset = 99,

	C_KillPlayer = 990,	// ����� ���� Ŀ�ǵ�, �׽�Ʈ ���� ���� �ʿ�
};
class Creature;
class ClientPacketHandler
{
public:
	static void HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len);

	// �ޱ�
	static void Handle_S_TEST(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Reset(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Hit(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Fire(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_MPRecover(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_SendMessage(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Teleport(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Gold(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Quests(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_QuestProcess(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_QuestComplete(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_QuestList(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_QuestState(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_ItemDrop(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_AddItem(ServerSessionRef session, BYTE* buffer, int32 len);

	// ������
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

	static SendBufferRef Make_C_Fire(uint64 objectId);
	static SendBufferRef Make_C_Hit(uint64 objectId, uint64 attackerId);
	static SendBufferRef Make_C_SendMessage(uint64 objectId, time_t time, string str, bool broadcast = true);
	static SendBufferRef Make_C_Revive(Protocol::ObjectInfo& objectInfo);
	static SendBufferRef Make_C_Quest(uint64 objectId, uint64 questId);
	static SendBufferRef Make_C_QuestFinish(uint64 objectId, uint64 questId);
	static SendBufferRef Make_C_QuestList();
	static SendBufferRef Make_C_Heal(uint64 objectId);
	static SendBufferRef Make_C_AddItem(uint64 objectId, int itemId, int itemCounts, Protocol::ITEM_TYPE itemType, int index);
	static SendBufferRef Make_C_EquipItem(uint64 objectId, int itemId, bool equip = true);
	static SendBufferRef Make_C_SyncInventory(uint64 objectID);
	static SendBufferRef Make_C_KillPlayer(uint64 objectID);
	static SendBufferRef Make_C_LeaveGame(uint64 objectID);

};

