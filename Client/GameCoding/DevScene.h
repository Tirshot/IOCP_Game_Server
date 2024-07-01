#pragma once
#include "Scene.h"

class Actor;
class UI;
class GameObject;
class Player;
class Item;

struct PQNode
{
	PQNode(int32 cost, Vec2Int pos) : cost(cost), pos(pos) {}

	// const �Ⱥ��̸� pqnode�� const ���� �Լ��� ȣ���Ϸ� ��
	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	Vec2Int pos;
};

struct PlayerQuestState
{
	int questId;
	Protocol::QUEST_STATE state;
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

	virtual void AddActor(shared_ptr<Actor> actor) override;
	virtual void RemoveActor(shared_ptr<Actor> actor) override;

	void ClearActors();
	void ClearUIs();

	void LoadMap();
	void LoadPlayer();
	void LoadMonster();
	void LoadNPC();
	void LoadItem();
	void LoadProjectile();
	void LoadEffect();
	void LoadTilemap();
	void LoadUI();
	void LoadSound();
	void LoadQuest();

	bool IsAnyPopUpVisible();

	template<typename T>
	shared_ptr<T> SpawnObject(Vec2Int pos)
	{
		// Ÿ�� ����, ������ Ÿ�ӿ� Ȯ��
		auto isGameObject = std::is_convertible_v<shared_ptr<T>, shared_ptr<GameObject>>;
		assert(isGameObject);

		shared_ptr<T> ret = make_shared<T>();
		ret->SetCellPos(pos, true);
		AddActor(ret);

		ret->BeginPlay();

		return ret;
	}

	template<typename T>
	shared_ptr<T> SpawnObjectAtRandomPos()
	{
		Vec2Int randPos = GetRandomEmptyCellPos();
		return SpawnObject<T>(randPos);
	}

	void SpawnItem(Protocol::ItemInfo info);

public:
	shared_ptr<GameObject> GetObjects(uint64 id);
	shared_ptr<Item> GetItem(uint64 id);
	shared_ptr<class Monster> GetMonster();

	// ��Ŷ �ڵ�
	void Handle_S_AddObject(Protocol::S_AddObject& pkt);
	void Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt);

	shared_ptr<Player> FindClosestPlayer(Vec2Int cellPos);
	bool FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth = 10);

	bool CanGo(Vec2Int cellPos);
	bool MonsterCanGo(Vec2Int cellPos);
	Vec2 ConvertPos(Vec2Int cellPos);
	Vec2Int GetRandomEmptyCellPos();

	// ����Ʈ ó��
	map<int, Protocol::QuestInfo>& GetQuests() { return _quests; }
	Protocol::QuestInfo& GetQuest(int questId) { return _quests[questId]; }
	void AddQuest(Protocol::QuestInfo info) { _quests.insert({ info.questid(), info }); }
	void SetQuests(int questid, Protocol::QuestInfo quest) { _quests.insert_or_assign(questid, quest); }
	void SetPlayerQuestState(int playerId, int questId, Protocol::QUEST_STATE state);
	Protocol::QUEST_STATE GetPlayerQuestState(int objectId, int questId);

private:
	void TickMonsterSpawn();

private:
	// ���� ���� ����
	const int32 DESIRED_MONSTER_COUNT = 1;
	int32 _monsterCount = 0;
	shared_ptr<class TilemapActor> _tilemapActor = nullptr;
	map<int, Protocol::QuestInfo> _quests;
	bool _questInitialized = false;
	int _alpha = 255;
	shared_ptr<Sprite> _black = nullptr;
};