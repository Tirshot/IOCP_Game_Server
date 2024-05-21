#include "pch.h"
#include "Inventory.h"

Inventory::Inventory()
{
}

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

}

void Inventory::AddItemToInventory(int itemID, int itemCounts, Protocol::ITEM_TYPE itemType, int index)
{
    if (itemID <= 0)
        return;

    bool found = false;
    int emptySlot = 0;

    _slots[index].itemID = itemID;
    _slots[index].itemCounts = itemCounts;

    if (_slots[index].itemCounts <= 0)
        _slots[index] = {};
}

