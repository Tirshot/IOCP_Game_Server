#pragma once
#include "Tilemap.h"

struct PQNode
{
	PQNode(int32 cost, Vec2Int pos) : cost(cost), pos(pos) {}

	// const �Ⱥ��̸� pqnode�� const ���� �Լ��� ȣ���Ϸ� ��
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
	Vec2Int GetRandomEmptySpawnCellPos();
	CreatureRef GetCreatureAt(Vec2Int cellPos);

public:
	void Handle_C_Move(Protocol::C_Move& pkt);

private:
	Tilemap _tilemap;

private:
	// ID�� �߱޹޾� Ȱ��
	map<uint64, PlayerRef> _players;
	map<uint64, MonsterRef> _monsters;
	map<uint64, NPCRef> _npcs;
	map<uint64, ArrowRef> _arrows;
	map<uint64, ItemRef> _items;
	map <uint64, GameObjectRef> _deleteObjects;
};

// �Ը� Ŀ���� �Ŵ��� Ŭ������ �ļ� ���
extern GameRoomRef GRoom;