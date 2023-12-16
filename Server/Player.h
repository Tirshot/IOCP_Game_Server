#pragma once
#include "Creature.h"
class Player : public Creature
{
	using Super = Creature;

public:
	Player();
	virtual ~Player();

	// Get, Set 함수를 안쓰기 위해 public, 추후 수정 필요
public:
	GameSessionRef session;
};

