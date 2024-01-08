#pragma once
#include "FlipbookActor.h"

class GameObject : public FlipbookActor
{
	using Super = FlipbookActor;
public:
	GameObject();
	virtual ~GameObject() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle() {}
	virtual void TickMove() {}
	virtual void TickSkill() {}
	virtual void TickHit() {}
	virtual void TickChat() {}
	virtual void TickSpin() {}
	virtual void TickSpinReady() {}
	virtual void UpdateAnimation() {}

public:
	void SetState(ObjectState state);
	Protocol::OBJECT_STATE_TYPE GetState() { return info.state(); }
	void SetDir(Dir dir);

	bool HasReachedDest();
	bool CanGo(Vec2Int cellPos);
	Dir GetLookAtDir(Vec2Int cellPos);

	void SetCellPos(Vec2Int cellPos, bool teleport = false);
	Vec2Int GetCellPos();
	Vec2Int GetFrontCellPos();

	int64 GetObjectID() { return info.objectid(); }
	void SetObjectID(int64 id) { info.set_objectid(id); }

public:
	void SetWeaponType(Protocol::WEAPON_TYPE weaponType);
	Protocol::WEAPON_TYPE GetWeaponType() { return info.weapontype(); }

protected:
	// 변화를 추적하기 위한 플래그
	bool _dirtyFlag = false;

public:
	// ProtoBuf
	Protocol::ObjectInfo info;
};

