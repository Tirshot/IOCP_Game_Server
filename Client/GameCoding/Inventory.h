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
	int ItemCount = 0;
	wstring Name = L"";
	wstring KorName = L"";
	wstring Type = L"";
	wstring Description = L"";
	class Sprite* Sprite = nullptr;
	RECT Rect = {};
};


class Inventory : public Panel
{
	using Super = Panel;

	DECLARE_SINGLE(Inventory);

	virtual ~Inventory();

public:
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	// �ܺο��� AddItem �Լ� �ߵ� -> SetItemSlot�� ���� _slots ��ȸ -> �κ��丮�� ������
private:
	void SetItemSlot(ITEM& slot);
	void SetSlotRects();

public:
	bool AddItem(int ItemId);
	void RemoveItem(ITEM& item);

	void ChangeItem(ITEM& itemFrom, ITEM& itemTo);

	int GetIndex(int x, int y);

	vector<wstring> findItemInfo(int itemID);

	int findItemByName(wstring Name);

public:
	wstring GetName(vector<wstring> row);
	wstring GetKorName(vector<wstring> row);
	wstring GetType(vector<wstring> row);
	wstring GetDescription(vector<wstring> row);
	class Sprite* GetSprite(wstring wstr);

protected:
	// nums : 8 x 5, slot : 28 x 28px
	vector<ITEM> _slots;
	vector<RECT> _rects;
	vector<vector<wstring>> _itemTable;
	class TextBox* _itemName = nullptr;
	class TextBox* _itemCount = nullptr;
	class TextBox* _itemDescription = nullptr;
	class Sprite* _background = nullptr;
	class Sprite* _itemSprite = nullptr;

private:
	int _slotSize = 28;

	// �巡�� �� ���
	ITEM* _selectedItem;
	ITEM* _destinatedItem;
	ITEM _temp;
	RECT _invenRect;	// �κ��丮 ���� üũ�� Rect
	POINT _mousePos;
};

