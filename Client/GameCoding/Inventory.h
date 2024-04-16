#pragma once
#include "Panel.h"

struct TABLE
{
	int ItemId;
	string Name;
	string Description;
};

struct ITEM
{
	int ItemId = 0;
	int Count = 0;
	wstring Name = L"";
	wstring Type = L"";
	wstring Description = L"";
	class Sprite* Sprite = nullptr;
	RECT Rect = {};
};


class Inventory : public Panel
{
	using Super = Panel;

public:
	Inventory();
	virtual ~Inventory();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	void AddItem(int ItemId);
	void RemoveItem(int ItemId);

	int GetIndex(int x, int y);

	vector<wstring> findItemInfo(int itemID);

	int findItemByName(wstring Name);

public:
	wstring GetName(vector<wstring> row);
	wstring GetType(vector<wstring> row);
	wstring GetDescription(vector<wstring> row);

protected:
	// nums : 8 x 5, slot : 28 x 28px
	vector<ITEM> _slots;
	vector<vector<wstring>> _itemTable;
	class TextBox* _textBox = nullptr;
	class Sprite* _background = nullptr;
	class Sprite* _itemSprite = nullptr;

private:

	int _slotSize = 28;

};

