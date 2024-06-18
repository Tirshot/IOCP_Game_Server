#pragma once
#include "GameObject.h"

class Creature;
class Arrow;
class Projectile : public GameObject
{
	using Super = GameObject;
public:
	Projectile();
	virtual ~Projectile() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle() override {};
	virtual void TickMove()  override {};
	virtual void TickSkill()  override {};
	virtual void UpdateAnimation()  override {};

public:
	void SetOwner(shared_ptr<Creature> owner) { _owner = owner; }
	shared_ptr<Creature> GetOwner() { return _owner; }

protected:
	shared_ptr<Creature> _owner = nullptr;
};

