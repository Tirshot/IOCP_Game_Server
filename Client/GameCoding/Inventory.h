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
	void SlotRectsPosUpdate(shared_ptr<RECT> rect);
	void SyncUseableItemToServer(int itemID, int counts);
	void SyncItemToServer(int itemID, int counts);
	void SyncEquips(int itemID, bool equip = true);

public:
	bool AddItem(shared_ptr<ITEM> item);
	bool AddItem(int ItemId);
	bool AddItem(int ItemId, int ItemCount);			// 인벤토리에 아이템이 없으면 더하기
	bool RemoveItem(shared_ptr<ITEM> item);
	bool RemoveItem(shared_ptr<ITEM> item, int ItemCount);
	bool RemoveItem(int itemId);
	bool RemoveItem(int itemId, int ItemCount);
	void SetItemCount(int itemId, int ItemCount);	// 아이템 개수 조정
	vector<shared_ptr<ITEM>> GetSlots() { return _slots; }
	RECT GetInvenRect() { return _invenRect; }

	void ChangeItem(ITEM& itemFrom, ITEM& itemTo);

	shared_ptr<ITEM> FindItemFromInventory(int itemId);
	int FindItemIndexFromInventory(int itemId);
	shared_ptr<ITEM> FindItemFromInventory(shared_ptr<ITEM> item);
	
	shared_ptr<ITEM> GetEquippedItem(wstring wstr);

	void EquipItem(shared_ptr<ITEM> item);
	void PressToSetQuickItem(ITEM slot);

	void OnPopClickAcceptDelegate();
	
	void ResetInventory();
	void AutoSyncInventory();

protected:
	// nums : 8 x 5, slot : 28 x 28px
	// 인벤토리
	vector<shared_ptr<ITEM>> _slots;
	vector<RECT> _rects;

	// 장착 슬롯
	vector<::pair<shared_ptr<RECT>, shared_ptr<ITEM>>> _equips;
	vector<RECT> _equipRects;
	
	shared_ptr<class TextBox> _itemName = nullptr;
	shared_ptr<class TextBox> _itemCount = nullptr;
	shared_ptr<class TextBox> _itemDescription = nullptr;
	shared_ptr<class Sprite> _background = nullptr;
	shared_ptr<class Sprite> _itemSprite = nullptr;
	shared_ptr<class Sprite> _descriptionSprite = nullptr;
	
private:
	int _slotSize = 28;
	bool _initialized = false;
	bool _revive = false;
	bool _isItemDropped = false;

	// 드래그 앤 드랍
	shared_ptr<ITEM> _selectedItem;
	shared_ptr<ITEM> _deleteItem = {};
	shared_ptr<ITEM> _destinatedItem;
	RECT _invenRect;	// 인벤토리 영역 체크용 Rect
	RECT _dragRect;		// 인벤토리 드래그 Rect
	RECT _equipRect;	// 장비창 영역 체크용 Rect
	shared_ptr<class AlertBox> _alert = nullptr;
	bool _isEquipedItem = false;
	shared_ptr<class MyPlayer> _owner = nullptr;
	float _sumTime = 0.f;
};

