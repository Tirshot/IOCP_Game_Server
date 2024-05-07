#pragma once
class Inventory
{
	struct Item
	{
		int itemID;
		int itemCounts;
	};

	DECLARE_SINGLE(Inventory);

public:
	virtual ~Inventory();

	virtual void Init();
	virtual void Update();

public:
	void AddItemToInventory(int itemID, int itemCounts);

private:
	vector<Item> _slots;
};

