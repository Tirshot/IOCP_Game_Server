#pragma once
#include "GameObject.h"
class Creature : public GameObject
{
	using Super = GameObject;
public:
	Creature();
	virtual ~Creature() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override {};
	virtual void TickMove()  override {};
	virtual void TickSkill()  override {};
	virtual void UpdateAnimation()  override {};

	virtual void OnDamaged(Creature* attacker);
	
	void KnockBack();
	float SetWait(float time) { return _wait = GetTickCount64() + time; }

	Protocol::OBJECT_TYPE GetType() { return this->info.objecttype(); }

protected:
	float _wait = 0;
	float _now = 0;
};

