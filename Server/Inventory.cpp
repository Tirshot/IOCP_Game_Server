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
	// 플레이어 접속 시 저장된 파일을 읽어 아이템 목록을 불러와 전송
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

