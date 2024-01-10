#pragma once
#include "Creature.h"
class NPC : public Creature
{
	using Super = Creature;
public:
	NPC();
	virtual ~NPC() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	virtual void TickIdle() override;
	virtual void TickMove()  override;

	virtual void UpdateAnimation()  override;

private:
	Flipbook* _flipbookIdle = nullptr;
	Flipbook* _flipbookMove = nullptr;
};

