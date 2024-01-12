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
	// Get, Set �Լ��� �Ⱦ��� ���� public, ���� ���� �ʿ�
public:
	GameSessionRef session;
};

