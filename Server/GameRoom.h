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

class Arrow;
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

private:
	// 몬스터 스폰 숫자
	const int32 DESIRED_MONSTER_COUNT = 15;
	int32 _monsterCount = 0;

public:
	void Handle_C_Move(Protocol::C_Move& pkt);

private:
	Tilemap _tilemap;

public:
	map<uint64, PlayerRef> GetPlayers() { return _players; }
	map<uint64, weak_ptr<Player>> GetTemps() { return _temps; }
	map<uint64, MonsterRef> GetMonsters() { return _monsters; }
	map<uint64, NPCRef> GetNPCs() { return _npcs; }
	map<uint64, ArrowRef> GetArrows() { return _arrows; }


private:
	// ID를 발급받아 활용
	map<uint64, PlayerRef> _players;
	map<uint64, weak_ptr<Player>> _temps;
	map<uint64, MonsterRef> _monsters;
	map<uint64, NPCRef> _npcs;
	map<uint64, ArrowRef> _arrows;
	map<uint64, ItemRef> _items;
	map <uint64, GameObjectRef> _deleteObjects;
};

// 규모가 커지면 매니저 클래스를 파서 사용
extern GameRoomRef GRoom;