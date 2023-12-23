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
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	void SetName(PlayerRef& player);
	void AddObject(GameObjectRef gameObject);
	void RemoveObject(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

public:
	PlayerRef FindClosestPlayer(Vec2Int cellPos);
	bool FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth = 10);
	bool CanGo(Vec2Int cellPos);
	Vec2Int GetRandomEmptyCellPos();
	CreatureRef GetCreatureAt(Vec2Int cellPos);

	// 화살도 작업할거라면 Creature를 살려서 플레이어와 몬스터가 상속받게 해야함.
public:
	void Handle_C_Move(Protocol::C_Move& pkt);

private:
	Tilemap _tilemap;

private:
	// ID를 발급받아 활용
	map<uint64, PlayerRef> _players;
	map<uint64, MonsterRef> _monsters;
};

// 규모가 커지면 매니저 클래스를 파서 사용
extern GameRoomRef GRoom;