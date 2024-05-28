#pragma once
#include "Tilemap.h"

struct PQNode
{
	PQNode(int32 cost, Vec2Int pos) : cost(cost), pos(pos) {}

	// const 안붙이면 pqnode가 const 붙은 함수를 호출하려 함
	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	Vec2Int pos;
};

struct PlayerQuestState
{
	int target;
	Protocol::QUEST_STATE state;
	int progress;
};

class Arrow;
class Quest;
class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom();
	virtual ~GameRoom();

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	GameObjectRef FindObject(uint64 id);
	weak_ptr<Player> FindObjectInTemp(uint64 id);
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	void SetName(PlayerRef& player);
	void AddObject(GameObjectRef gameObject);
	void RemoveObject(uint64 id);
	void RemoveTemp(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

public:
	PlayerRef FindClosestPlayer(Vec2Int cellPos);
	bool FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth = 10);
	bool CanGo(Vec2Int cellPos);
	bool MonsterCanGo(Vec2Int cellPos);
	Vec2Int GetRandomEmptyCellPos();
	Vec2Int GetRandomEmptySpawnCellPos();
	CreatureRef GetCreatureAt(Vec2Int cellPos);

	void RandomMonsterSpawn();

public:
	Protocol::QuestInfo& GetQuest(int questId);

private:
	// 몬스터 스폰 숫자
	const int32 DESIRED_MONSTER_COUNT = 10;
	int32 _monsterCount = 0;

public:
	void Handle_C_Move(Protocol::C_Move& pkt);

private:
	Tilemap _tilemap;

public:
	map<uint64, PlayerRef> GetPlayers() { return _players; }
	PlayerRef GetPlayer(uint64 objectId) { return _players[objectId]; }
	map<uint64, weak_ptr<Player>> GetTemps() { return _temps; }
	map<uint64, MonsterRef> GetMonsters() { return _monsters; }
	map<uint64, NPCRef> GetNPCs() { return _npcs; }
	map<uint64, ArrowRef> GetArrows() { return _arrows; }
	map<uint64, Protocol::QuestInfo> GetQuests() { return _quests; }
	map<uint64, InventoryRef> GetInventorys() { return _inventorys; }
	map<uint64, ItemRef> GetItems() { return _items; }
	map<uint64, TriggerRef> GetTriggers() { return _triggers; }
	InventoryRef GetInventory(int objectId) { return _inventorys[objectId]; }
	void AddQuest(class Quest quest);
	int GetQuestsSize() { return _quests.size(); }
	void AddItemToPlayer(int objectId, int itemId, int itemCounts, Protocol::ITEM_TYPE itemType, int index);
	void EquipItemToPlayer(int objectId, int itemId, bool equip);
	void SetQuestStates(uint64 objectId, int questId, Protocol::QUEST_STATE state);
	map<int, PlayerQuestState> GetQuestsStatesByID(uint64 objectId);
	PlayerQuestState GetQuestsStates(uint64 objectId, int questId) { return _questsStates[objectId][questId]; }
	map<int, PlayerQuestState> GetQuestsStates(uint64 objectId) { return _questsStates[objectId]; }
	void SetQuestStateProgress(uint64 objectId, int questId, int progress);

private:
	// ID를 발급받아 활용
	map<uint64, PlayerRef> _players;
	map<uint64, weak_ptr<Player>> _temps;
	map<uint64, MonsterRef> _monsters;
	map<uint64, NPCRef> _npcs;
	map<uint64, ArrowRef> _arrows;
	map<uint64, ItemRef> _items;
	map<uint64, Protocol::QuestInfo> _quests;		// 퀘스트 관리
	map<uint64, map<int, PlayerQuestState>> _questsStates;	// 플레이어 별 퀘스트 상태 관리
	map<uint64, InventoryRef> _inventorys;
	map<uint64, TriggerRef> _triggers;
	map<uint64, GameObjectRef> _deleteObjects;
};

// 규모가 커지면 매니저 클래스를 파서 사용
extern GameRoomRef GRoom;