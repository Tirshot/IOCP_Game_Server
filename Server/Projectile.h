#pragma once
#include "GameObject.h"

class Creature;
class Projectile : public GameObject
{
	using Super = GameObject;
public:
	Projectile();
	virtual ~Projectile() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

protected:
	virtual void TickIdle() override {};
	virtual void TickMove()  override {};
	virtual void TickSkill()  override {};

public:
	void SetOwner(CreatureRef owner) { _owner = owner; }
	CreatureRef& GetOwner() { return _owner; }
	bool IsHit() { return _hit; }

protected:
	CreatureRef _owner = nullptr;

	// 삭제를 위한 적중 여부
	bool _hit = false;
};
