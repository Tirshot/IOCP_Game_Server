#include "pch.h"
#include "Inventory.h"
#include "ItemManager.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"

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
    _equips.assign(5, { 0, 0 });

    AddItemToInventory(1, 1, Protocol::ITEM_TYPE_WEARABLE, 0);
    AddItemToInventory(2, 1, Protocol::ITEM_TYPE_WEARABLE, 0);
    AddItemToInventory(3, 1, Protocol::ITEM_TYPE_WEARABLE, 0);
}

void Inventory::Update()
{
    for (auto& slot : _slots)
    {
        if (slot.itemCounts <= 0)
        {
            slot = {};
        }
    }
}

void Inventory::AddItemToInventory(int itemID, int itemCounts, Protocol::ITEM_TYPE itemType, int index, bool reset)
{
    if (itemID <= 0)
        return;

    bool found = false;

    for (auto& slot : _slots)
    {
        if (slot.itemID == itemID)
        {
            found = true;
            slot.itemID = itemID;
            slot.itemCounts = itemCounts;
            return;
        }
    }

    if (found == false)
    {
        for (auto& slot : _slots)
        {
            if (slot.itemID == 0)
            {
                slot.itemID = itemID;
                slot.itemCounts = itemCounts;
                return;
            }
        }
    }
}

void Inventory::EquipItem(int itemID, bool equip)
{
    if (itemID <= 0)
        return;

    auto subType = GET_SINGLE(ItemManager)->GetItemSubType(itemID);

    // ����� info�� ����
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

void Inventory::SyncToClient(uint64 objectID)
{
    if (_ownerId != objectID)
        return;

    // ������ ��ȸ�Ͽ� Ŭ���̾�Ʈ�� ����
    for (const auto& slot : _slots)
    {
        int itemID = slot.itemID;
        int itemCounts = slot.itemCounts;

        {
            SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddItem(objectID, itemID, itemCounts);
            PlayerRef player = GRoom->GetPlayer(objectID);

            if (player)
                 player->session->Send(sendBuffer);
        }
    }
}
