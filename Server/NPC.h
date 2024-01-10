#pragma once
#include "Creature.h"
class NPC : public Creature
{
	using Super = Creature;

public:
	NPC();
	virtual ~NPC();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	virtual void TickIdle() override;
	virtual void TickMove() override;
};

