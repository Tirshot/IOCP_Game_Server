#pragma once

class Sprite;
class Inventory;
class QuickSlot;
class ItemManager
{
	DECLARE_SINGLE(ItemManager);

public:
	void Init();
	void Clear();
	void Tick();

public:
	vector<wstring> FindItemInfo(int itemID);
	int FindItemIDByName(wstring Name);

	ITEM GetItem(int itemID);

	shared_ptr<class Inventory> GetInventory() { return _inventory; }
	bool AddItemToInventory(int itemId);
	bool AddItemToInventory(int itemId, int counts);
	bool SetItemToInventory(int itemId, int counts);
	bool RemoveItemFromInventory(int itemId);

	shared_ptr<class QuickSlot> GetQuickSlot() { return _quickSlot; }
	void SetItemToQuickSlot(struct shared_ptr<ITEM> item, int index);

	int GetQuickSlotSelectedIndex();
	wstring GetQuickSlotSelectedSubType();

	void EquipItem(shared_ptr<ITEM> item);
	void QuickEquipItem(int itemID);

	bool IsInventoryFull();
	int GetEmptySlots();

	shared_ptr<ITEM> FindItemFromInventory(int itemId);
	void SyncToServer();
	void OpenInventory();

	void ResetInventory();

public:
	wstring GetName(vector <wstring> row);
	wstring GetKorName(vector <wstring> row);
	wstring GetType(vector <wstring> row);
	wstring GetSubType(vector <wstring> row);
	wstring GetDescription(vector <wstring> row);
	int GetPrice(vector <wstring> row);
	shared_ptr<Sprite> GetSprite(wstring wstr);
	shared_ptr<Sprite> GetSprite(int itemID);

private:
	shared_ptr<Inventory> _inventory = nullptr;
	shared_ptr<QuickSlot> _quickSlot = nullptr;
};

