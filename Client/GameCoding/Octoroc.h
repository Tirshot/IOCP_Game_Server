#pragma once
#include "Monster.h"
class Octoroc :  public Monster
{
	using Super = Monster;
public:
	Octoroc();
	virtual ~Octoroc() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override;
	virtual void TickMove()  override;
	virtual void TickSkill()  override;
	virtual void TickHit() override;
	virtual void UpdateAnimation() override;

public:
	void MakeRock();

protected:
	shared_ptr<Flipbook> _flipbookIdle[4] = {};
	shared_ptr<Flipbook> _flipbookMove[4] = {};
	shared_ptr<Flipbook> _flipbookSkill[4] = {};

	float _waitSeconds = 0.f;
};

