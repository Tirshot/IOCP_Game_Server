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

public:
	void MakeArrow();
	void Teleport();
	// Get, Set 함수를 안쓰기 위해 public, 추후 수정 필요
public:
	GameSessionRef session;
};

