#pragma once
#include "Inventory.h"
class Sprite;
class QuickSlot :  public Panel
{
	using Super = Panel;

public:
	virtual ~QuickSlot();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	void SetQuickSlot(shared_ptr<ITEM> item, int index);

	int GetSelectedIndex() { return _selectedIndex; }
	wstring GetSelectedSubType() { return _selectedItemSubType; }

public:
	void SetPressedButton();

protected:
	vector<ITEM> _slots;
	shared_ptr<Sprite> _slotBackground = nullptr;
	shared_ptr<Sprite> _slotSelectedSprite = nullptr;
	shared_ptr<ITEM> _selectedItem = nullptr;
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

