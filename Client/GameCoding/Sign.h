#pragma once
#include "NPC.h"
class Sign : public NPC
{
	using Super = Creature;
public:
	Sign();
	virtual ~Sign() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override;
	virtual void TickMove()  override;

	virtual void UpdateAnimation()  override;

protected:
	Flipbook* _flipbookIdle[3] = {};
	Flipbook* _flipbookMove[3] = {};
};

