#include "pch.h"
#include "Inventory.h"

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	// �÷��̾� ���� �� ����� ������ �о� ������ ����� �ҷ��� ����
    _slots.assign(40, {0, 0});
}

void Inventory::Update()
{
    // _slots�� ��ȸ�ϸ鼭 itemCounts�� 0�� ������ ����
    if (_slots.size() == 0)
        return;

    //for (auto it = _slots.begin(); it != _slots.end();)
    //{
    //    if (it->itemCounts == 0)
    //    {
    //        it = _slots.erase(it);
    //    }
    //    else
    //    {
    //        ++it;
    //    }
    //}
}

void Inventory::AddItemToInventory(int itemID, int itemCounts)
{
    Item item = {};
    item.itemID = itemID;
    item.itemCounts = itemCounts;

	_slots.push_back(item);
}

