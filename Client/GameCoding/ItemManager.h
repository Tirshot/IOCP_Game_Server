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

	ITEM& GetItem(int itemID);

	class Inventory* GetInventory() { return _inventory; }
	bool AddItemToInventory(int itemId);
	bool AddItemToInventory(int itemId, int counts);
	bool SetItemToInventory(int itemId, int counts);
	bool RemoveItemFromInventory(int itemId);

	class QuickSlot* GetQuickSlot() { return _quickSlot; }
	void SetItemToQuickSlot(struct ITEM* item, int index);

	int GetQuickSlotSelectedIndex();
	wstring GetQuickSlotSelectedSubType();

	void EquipItem(ITEM item);

	bool IsInventoryFull();
	int GetEmptySlots();

	ITEM* FindItemFromInventory(int itemId);
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
	Sprite* GetSprite(wstring wstr);
	Sprite* GetSprite(int itemID);

private:
	Inventory* _inventory = nullptr;
	QuickSlot* _quickSlot = nullptr;
};

