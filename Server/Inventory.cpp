#include "pch.h"
#include "Inventory.h"

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
    // _slots를 순회하면서 itemCounts가 0인 슬롯을 제거
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

