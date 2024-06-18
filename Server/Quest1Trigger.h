#pragma once
#include "Trigger.h"
class Quest1Trigger : public Trigger
{
	using Super = Trigger;

public:
	Quest1Trigger();
	virtual ~Quest1Trigger();

public:
	virtual void Tick();

public:
	virtual bool IsPlayerOnTile() override;
};

