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
	// Get, Set �Լ��� �Ⱦ��� ���� public, ���� ���� �ʿ�
public:
	GameSessionRef session;

private:
	// �ʴ� ���� ȸ�� ���
	uint64 _now = 0;
	uint64 _prev = 0;
};

