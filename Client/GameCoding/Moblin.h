#pragma once
#include "Monster.h"
class Moblin : public Monster
{
	using Super = Monster;
public:
	Moblin();
	virtual ~Moblin() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override;
	virtual void TickMove()  override;
	virtual void TickSkill()  override;
	virtual void TickHit() override;
	virtual void UpdateAnimation() override;

protected:
	shared_ptr<Flipbook> _flipbookIdle[4] = {};
	shared_ptr<Flipbook> _flipbookMove[4] = {};
	shared_ptr<Flipbook> _flipbookAttack[4] = {};
	shared_ptr<Flipbook> _flipbookHit[4] = {};

	shared_ptr<Player> _target = nullptr;
	float _waitSeconds = 0.f;
};

