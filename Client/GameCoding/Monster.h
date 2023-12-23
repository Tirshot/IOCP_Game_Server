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
	virtual void UpdateAnimation()  override;

	void KnockBack();

protected:
	Flipbook* _flipbookMove[4] = {};
	Flipbook* _flipbookHit[4] = {};
	float _waitSeconds = 0.f;

	Player* _target = nullptr; // 임시!!!!!, 스마트 포인터 사용하기 or id 발급하기
};

