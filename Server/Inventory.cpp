#include "pch.h"
#include "Inventory.h"
#include "ItemManager.h"
#include "GameRoom.h"
#include "Player.h"

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
    _equips.assign(5, { 0, 0 });
}

void Inventory::Update()
{

}

void Inventory::AddItemToInventory(int itemID, int itemCounts, Protocol::ITEM_TYPE itemType, int index, bool reset)
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

void Inventory::EquipItem(int itemID, bool equip)
{
    if (itemID <= 0)
        return;

    auto subType = GET_SINGLE(ItemManager)->GetItemSubType(itemID);

    // 무기는 info로 관리
    PlayerRef player = static_pointer_cast<Player>(GRoom->FindObject(_ownerId));

    if (player)
        _equips[0].itemID = player->info.weapontype();

    switch (subType)
    {
    case Protocol::WEARABLE_TYPE_HELMET:
    {
        if (equip)
        {
            _equips[1].itemID = itemID;
            _equips[1].itemCounts = 1;
        }
        else
        {
            _equips[1].itemID = itemID;
            _equips[1].itemCounts = 0;
        }
        break;
    }
    case Protocol::WEARABLE_TYPE_ARMOR:
    {
        if (equip)
        {
            _equips[2].itemID = itemID;
            _equips[2].itemCounts = 1;
        }
        else
        {
            _equips[2].itemID = itemID;
            _equips[2].itemCounts = 0;
        }
        break;
    }
    case Protocol::WEARABLE_TYPE_PANTS:
    {
        if (equip)
        {
            _equips[3].itemID = itemID;
            _equips[3].itemCounts = 1;
        }
        else
        {
            _equips[3].itemID = itemID;
            _equips[3].itemCounts = 0;
        }
        break;
    }
    case Protocol::WEARABLE_TYPE_BOOTS:
    {
        if (equip)
        {
            _equips[4].itemID = itemID;
            _equips[4].itemCounts = 1;
        }
        else
        {
            _equips[4].itemID = itemID;
            _equips[4].itemCounts = 0;
        }
        break;
    }
    }
}

