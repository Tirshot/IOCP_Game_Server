#pragma once
class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update();
	
public:
	// ���� ���� �ڵ�
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

	void OnDamaged(GameObjectRef attacker);

	// ������Ʈ�� �Ѱ� ����
	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster();


public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

private:
	// id �ο�
	static atomic<uint64> s_idGenerator;
};
