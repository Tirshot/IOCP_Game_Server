#pragma once
#include "Panel.h"

class AlertBox;
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

public:
	shared_ptr<AlertBox> MakeAlertBox(Vec2 pos, Vec2Int size, void (Inventory::* func)() = nullptr, bool twoButtons = true);

private:
	void SetItemSlot(ITEM& slot);
	void SetEquipSlotRects();
	void SlotRectsPosUpdate(RECT rect);
	void AddUseableItemToServer(int itemID, int counts);
	void AddItemToServer(int itemID, int counts);
	void SyncEquips(int itemID, bool equip = true);
	void ApplyStatus();

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
	void SyncUseableItem();

	shared_ptr<ITEM> FindItemFromInventory(int itemId);
	int FindItemIndexFromInventory(int itemId);
	shared_ptr<ITEM> FindItemFromInventory(shared_ptr<ITEM> item);
	
	ITEM GetEquippedItem(wstring wstr);

	void EquipItem(shared_ptr<ITEM> item);
	void UnEquipItem(shared_ptr<ITEM> item);

	void QuickEquipItem(int itemID);
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
	vector<::pair<RECT, ITEM>> _equips;
	vector<RECT> _equipRects;
	
	shared_ptr<class TextBox> _itemName = nullptr;
	shared_ptr<class TextBox> _itemDescription = nullptr;
	shared_ptr<class Sprite> _background = nullptr;
	shared_ptr<class Sprite> _itemSprite = nullptr;
	shared_ptr<class Sprite> _descriptionSprite = nullptr;
	
private:
	int _slotSize = 28;
	bool _initialized = false;
	bool _revive = false;

	// 드래그 앤 드랍
	shared_ptr<ITEM> _selectedItem;
	shared_ptr<ITEM> _deleteItem = {};
	shared_ptr<ITEM> _destinatedItem;
	RECT _invenRect;	// 인벤토리 영역 체크용 Rect
	RECT _dragRect;		// 인벤토리 드래그 Rect
	RECT _equipRect;	// 장비창 영역 체크용 Rect
	bool _isEquipedItem = false;
	shared_ptr<class MyPlayer> _owner = nullptr;
	float _sumTime = 0.f;
};

