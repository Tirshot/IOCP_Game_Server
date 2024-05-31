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
	void SyncEquips(int itemID, bool equip = true);

public:
	bool AddItem(ITEM* item);
	bool AddItem(int ItemId);
	bool AddItem(int ItemId, int ItemCount);			// �κ��丮�� �������� ������ ���ϱ�
	bool RemoveItem(ITEM* item);
	bool RemoveItem(ITEM* item, int ItemCount);
	bool RemoveItem(int itemId);
	bool RemoveItem(int itemId, int ItemCount);
	void SetItemCount(int itemId, int ItemCount);	// ������ ���� ����
	vector<ITEM> GetSlots() { return _slots; }
	RECT GetInvenRect() { return _invenRect; }

	void ChangeItem(ITEM& itemFrom, ITEM& itemTo);

	ITEM* FindItemFromInventory(int itemId);
	int FindItemIndexFromInventory(int itemId);
	ITEM* FindItemFromInventory(ITEM* item);
	
	ITEM* GetEquippedItem(wstring wstr);

	void EquipItem(ITEM* item);
	void PressToSetQuickItem(ITEM& slot);

	void OnPopClickAcceptDelegate();
	
	void ResetInventory();
	void AutoSyncInventory();

protected:
	// nums : 8 x 5, slot : 28 x 28px
	// �κ��丮
	vector<ITEM> _slots;
	vector<RECT> _rects;

	// ���� ����
	vector<::pair<RECT, ITEM>> _equips;
	vector<RECT> _equipRects;
	
	class TextBox* _itemName = nullptr;
	class TextBox* _itemCount = nullptr;
	class TextBox* _itemDescription = nullptr;
	class Sprite* _background = nullptr;
	class Sprite* _itemSprite = nullptr;
	class Sprite* _descriptionSprite = nullptr;
	
private:
	int _slotSize = 28;
	bool _initialized = false;
	bool _revive = false;
	bool _isItemDropped = false;

	// �巡�� �� ���
	ITEM* _selectedItem;
	ITEM* _deleteItem = {};
	ITEM* _destinatedItem;
	RECT _invenRect;	// �κ��丮 ���� üũ�� Rect
	RECT _dragRect;		// �κ��丮 �巡�� Rect
	RECT _equipRect;	// ���â ���� üũ�� Rect
	class AlertBox* _alert = nullptr;
	bool _isEquipedItem = false;
	class MyPlayer* _owner = nullptr;
	float _sumTime = 0.f;
};

