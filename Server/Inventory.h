#pragma once
#include "GameObject.h"
class Inventory : public GameObject
{
	struct Item
	{
		int itemID;
		int itemCounts;
	};

public:
	Inventory();
	virtual ~Inventory();

	virtual void Init();
	virtual void Update();

	void SetOwner(int64 id) { _ownerId = id; }

public:
	void AddItemToInventory(int itemID, int itemCounts, Protocol::ITEM_TYPE itemType, int index, bool reset = false);
	void EquipItem(int itemID, bool equip);

private:
	vector<Item> _slots;
	vector<Item> _equips; // 0 : Weapon, 1 : Helmet, 2 : Armor, 3 : Pants, 4 : Boots
	int64 _ownerId;
};

