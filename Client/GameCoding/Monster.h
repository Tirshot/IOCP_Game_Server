#pragma once
#include "Creature.h"
class Player;

class Monster : public Creature
{
	using Super = Creature;
public:
	Monster();
	virtual ~Monster() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override;
	virtual void TickMove()  override;
	virtual void TickSkill()  override;
	virtual void TickHit() override;
	virtual void UpdateAnimation()  override;

protected:
	shared_ptr<Flipbook> _flipbookMove[4] = {};
	shared_ptr<Flipbook> _flipbookHit[4] = {};

	shared_ptr<Player> _target = nullptr; // 임시!!!!!, 스마트 포인터 사용하기 or id 발급하기
	float _waitSeconds = 0.f;
};

