#pragma once
#include "Inventory.h"
class Sprite;
class QuickSlot :  public Panel
{
	using Super = Panel;
	DECLARE_SINGLE(QuickSlot);

public:
	virtual ~QuickSlot();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	void SetQuickSlot(ITEM* item, int index);

	int GetSelectedIndex() { return _selectedIndex; }
	wstring GetSelectedSubType() { return _selectedItemSubType; }

public:
	void SetPressedButton();

protected:
	vector<ITEM> _slots;
	Sprite* _slotBackground = nullptr;
	Sprite* _slotSelectedSprite = nullptr;
	ITEM* _selectedItem = nullptr;
	int _selectedIndex = 0;
	wstring _selectedItemSubType = L"";

private:
	int _slotSize = 60;
	int _size_x = 0;
	int _size_y = 0;

	int _center_x = 0;
	int _center_y = 0;

	int _pressedButton = 1;
};

