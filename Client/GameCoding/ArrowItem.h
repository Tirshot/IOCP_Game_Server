#pragma once
#include "Item.h"

class TriggerActor;
class ArrowItem :  public Item
{
	using Super = Item;
public:
	ArrowItem();
	virtual ~ArrowItem() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle();

private:
	TriggerActor* _trigger = nullptr;
	Flipbook* _flipbookArrow = nullptr;
};

