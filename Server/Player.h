#pragma once
#include "Creature.h"
class Player : public Creature
{
	using Super = Creature;

public:
	Player();
	virtual ~Player();

	// Get, Set �Լ��� �Ⱦ��� ���� public, ���� ���� �ʿ�
public:
	GameSessionRef session;
};

