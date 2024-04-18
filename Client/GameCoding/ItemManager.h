#pragma once

class ItemManager
{
	DECLARE_SINGLE(ItemManager);

public:
	void Init();

public:
	vector<vector<wstring>> GetItemTable() { return _itemTable; }
	vector<wstring> FindItemInfo(int itemID);
	int FindItemIDByName(wstring Name);

	class Inventory* GetInventory() { return _inventory; }
	bool AddItemToInventory(int itemId);
	bool RemoveItemFromInventory(int itemId);

	class QuickSlot* GetQuickSlot() { return _quickSlot; }
	void SetItemToQuickSlot(class ITEM* item, int index);

public:
	wstring GetName(vector <wstring> row);
	wstring GetKorName(vector <wstring> row);
	wstring GetType(vector <wstring> row);
	wstring GetSubType(vector <wstring> row);
	wstring GetDescription(vector <wstring> row);
	class Sprite* GetSprite(wstring wstr);

private:
	vector<vector<wstring>> _itemTable;
	class Inventory* _inventory = nullptr;
	class QuickSlot* _quickSlot = nullptr;
};

