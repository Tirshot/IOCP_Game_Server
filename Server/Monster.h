#pragma once
#include "Creature.h"
class Monster : public Creature
{
	using Super = Creature;

public:
	Monster();
	virtual ~Monster();

	virtual void Update();

private:

};

