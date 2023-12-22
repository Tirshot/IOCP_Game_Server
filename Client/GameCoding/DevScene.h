#pragma once
#include "Scene.h"

class Actor;
class UI;
class GameObject;
class Player;

struct PQNode
{
	PQNode(int32 cost, Vec2Int pos) : cost(cost), pos(pos) {}

	// const 안붙이면 pqnode가 const 붙은 함수를 호출하려 함
	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	Vec2Int pos;
};

class DevScene :  public Scene
{
	using Super = Scene;
public:
	DevScene();
	virtual ~DevScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual void AddActor(Actor* actor) override;
	virtual void RemoveActor(Actor* actor) override;

	void LoadMap();
	void LoadPlayer();
	void LoadMonster();
	void LoadProjectile();
	void LoadEffect();
	void LoadTilemap();
	void LoadUI();

	template<typename T>
	T* SpawnObject(Vec2Int pos)
	{
		// 타입 추측, 컴파일 타임에 확인
		auto isGameObject = std::is_convertible_v<T*, GameObject*>;
		assert(isGameObject);

		T* ret = new T();
		ret->SetCellPos(pos, true);
		AddActor(ret);

		ret->BeginPlay();

		return ret;
	}

	template<typename T>
	T* SpawnObjectAtRandomPos()
	{
		Vec2Int randPos = GetRandomEmptyCellPos();
		return SpawnObject<T>(randPos);
	}

public:
	GameObject* GetObject(uint64 id);

	// 패킷 핸들
	void Handle_S_AddObject(Protocol::S_AddObject& pkt);
	void Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt);

	Player* FindClosestPlayer(Vec2Int cellPos);
	bool FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth = 10);

	bool CanGo(Vec2Int cellPos);
	Vec2 ConvertPos(Vec2Int cellPos);
	Vec2Int GetRandomEmptyCellPos();

private:
	void TickMonsterSpawn();

private:
	// 몬스터 스폰 숫자
	const int32 DESIRED_MONSTER_COUNT = 1;
	int32 _monsterCount = 0;
	class TilemapActor* _tilemapActor = nullptr;
	vector<UI*> _uis;
};