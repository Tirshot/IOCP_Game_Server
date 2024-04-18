#pragma once
#include "Panel.h"
class Sprite;
class WeaponSlot : public Panel
{
	using Super = Panel;

public:
	WeaponSlot();
	virtual ~WeaponSlot();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	/*int16 HighlightSlot();*/

	/*void AddSlot(Sprite* sprite) { _slots.insert({ _num++, sprite}); }*/

protected:
	vector<ITEM> _slots;
	Sprite* _slotBackground = nullptr;
	Sprite* _slotSelected = nullptr;
};