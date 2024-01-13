#pragma once
#include "Creature.h"
class Player : public Creature
{
	using Super = Creature;

public:
	Player();
	virtual ~Player();

	virtual void Init();
	virtual void Update();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateTeleport();

public:
	void MakeArrow();
	void Teleport();
	// Get, Set 함수를 안쓰기 위해 public, 추후 수정 필요
public:
	GameSessionRef session;

private:
	// 초당 마나 회복 계산
	uint64 _now = 0;
	uint64 _prev = 0;
};

