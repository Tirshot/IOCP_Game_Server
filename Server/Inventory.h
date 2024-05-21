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
	void AddItemToInventory(int itemID, int itemCounts, Protocol::ITEM_TYPE itemType, int index);

private:
	vector<Item> _slots;
	int64 _ownerId;
};

