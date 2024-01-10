#pragma once
#include "NPC.h"
class Sign : public NPC
{
	using Super = NPC;

public:
	Sign();
	virtual ~Sign();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	virtual void TickIdle() override;
	virtual void TickMove() override;
};

