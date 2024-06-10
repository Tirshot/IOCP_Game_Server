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
	shared_ptr<Flipbook> _flipbookIdle = nullptr;
	shared_ptr<Flipbook> _flipbookMove = nullptr;
};

