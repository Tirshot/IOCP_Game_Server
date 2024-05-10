#pragma once
#include "Panel.h"

class Inventory : public Panel
{
	using Super = Panel;

public:
	Inventory();
	virtual ~Inventory();

public:
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	void SetItemSlot(ITEM& slot);
	void SetEquipSlotRects();
	void SlotRectsPosUpdate(RECT* rect);
	void SyncUseableItemToServer(int itemID, int counts);
	void SyncItemToServer(int itemID, int counts);

public:
	bool AddItem(ITEM* item);
	bool AddItem(int ItemId);
	bool AddItem(int ItemId, int ItemCount);
	bool RemoveItem(ITEM* item);
	bool RemoveItem(ITEM* item, int ItemCount);
	bool RemoveItem(int itemId);
	bool RemoveItem(int itemId, int ItemCount);
	void SetItemCount(int itemId, int ItemCount);
	vector<ITEM> GetSlots() { return _slots; }

	void ChangeItem(ITEM& itemFrom, ITEM& itemTo);

	ITEM* FindItemFromInventory(int itemId);
	
	ITEM* GetEquippedItem(wstring wstr);

	void EquipItem(ITEM* item);
	void PressToSetQuickItem(ITEM& slot);

	void OnPopClickAcceptDelegate();

protected:
	// nums : 8 x 5, slot : 28 x 28px
	// 인벤토리
	vector<ITEM> _slots;
	vector<RECT> _rects;

	// 장착 슬롯
	vector<::pair<RECT, ITEM>> _equips;
	vector<RECT> _equipRects;
	
	class TextBox* _itemName = nullptr;
	class TextBox* _itemCount = nullptr;
	class TextBox* _itemDescription = nullptr;
	class Sprite* _background = nullptr;
	class Sprite* _itemSprite = nullptr;
	
private:
	int _slotSize = 28;
	bool _initialized = false;
	bool _isItemDropped = false;

	// 드래그 앤 드랍
	ITEM* _selectedItem;
	ITEM* _deleteItem = {};
	ITEM* _destinatedItem;
	RECT _invenRect;	// 인벤토리 영역 체크용 Rect
	RECT _dragRect;		// 인벤토리 드래그 Rect
	class AlertBox* _alert = nullptr;
};

