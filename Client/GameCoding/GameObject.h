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
	virtual void UpdateAnimation() {}

public:
	void SetState(ObjectState state);
	void SetDir(Dir dir);

	bool HasReachedDest();
	bool CanGo(Vec2Int cellPos);
	Dir GetLookAtDir(Vec2Int cellPos);

	void SetCellPos(Vec2Int cellPos, bool teleport = false);
	Vec2Int GetCellPos() { return _cellPos; }
	Vec2Int GetFrontCellPos();

protected:
	Vec2Int _cellPos = {};
	Vec2 _speed = {};
	Dir _dir = DIR_DOWN;
	ObjectState _state = ObjectState::Idle;
};

