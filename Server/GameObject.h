#pragma once

class GameObject
{
public:
	GameObject();
	virtual ~GameObject ();

	virtual void BeginPlay() {};
	virtual void Tick() {};

public:
	virtual void TickIdle() {};
	virtual void TickMove() {};
	virtual void TickSkill() {};
	
public:
	// 전투 관련 코드
	void SetState(ObjectState state , bool broadcast = false);
	void SetDir(Dir dir, bool broadcast = false);

	bool HasReachedDest();
	bool CanGo(Vec2Int cellPos);
	Dir GetLookAtDir(Vec2Int cellPos);

	void SetCellPos(Vec2Int cellPos, bool broadcast = false);
	Vec2Int GetCellPos() { return Vec2Int{ info.posx(), info.posy() }; }
	Vec2Int GetFrontCellPos();

	int64 GetObjectID() { return info.objectid(); }
	void SetObjectID(int64 id) { info.set_objectid(id); }

	void BroadcastMove();

	Protocol::OBJECT_TYPE GetType() { return info.objecttype(); }
	// 오브젝트를 총괄 관리
	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster(Protocol::MONSTER_TYPE monsterType);
	static NPCRef CreateNPC();
	static SignRef CreateSign();
	static ItemRef CreateItem();
	static ArrowRef CreateArrow();
	static InventoryRef CreateInventory(PlayerRef player);
	static TriggerRef CreateTrigger(uint64 questId);

public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

protected:
	// id 부여
	static atomic<uint64> s_idGenerator;
};

