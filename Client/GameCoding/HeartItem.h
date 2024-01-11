#pragma once
#include "Item.h"
class TriggerActor;
class HeartItem :  public Item
{
	using Super = Item;
public:
	HeartItem();
	virtual ~HeartItem() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle();
	virtual void UpdateAnimation();

private:
	TriggerActor* _trigger = nullptr;
	Flipbook* _flipbookIdle[4] = {};
};

