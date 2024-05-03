#include "pch.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "QuickSlot.h"

void ItemManager::Init()
{
    DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
    if (scene == nullptr)
        return;

    _inventory = scene->FindUI<Inventory>(scene->GetUIs());
    _quickSlot = scene->FindUI<QuickSlot>(scene->GetUIs());
}

vector<wstring> ItemManager::FindItemInfo(int itemID)
{
    const auto ItemTable = GET_SINGLE(ResourceManager)->GetItemTable();

    for (auto& row : ItemTable)
    {
        if (row.empty())
            return {};

        int result;
        std::wistringstream(row[0]) >> result;

        if (result == itemID)
        {
            return row;
        }
    }

    // 실패하면 무엇을 반환할까
    return {};
}

int ItemManager::FindItemIDByName(wstring Name)
{
    const auto ItemTable = GET_SINGLE(ResourceManager)->GetItemTable();

    for (auto& row : ItemTable)
    {
        if (row.empty())
            return -1;

        if (row[1] == Name)
        {
            return stoi(row[0]);
        }
    }

    return -1;
}

ITEM& ItemManager::GetItem(int itemID)
{
    // 아이템 정보를 찾음
    vector<wstring> ItemInfo = GET_SINGLE(ItemManager)->FindItemInfo(itemID);

    // ITEM 객체를 동적으로 할당
    ITEM* item = new ITEM;

    // 아이템 정보 할당
    item->ItemId = itemID;
    item->ItemCount = 1;
    item->Name = GET_SINGLE(ItemManager)->GetName(ItemInfo);
    item->KorName = GET_SINGLE(ItemManager)->GetKorName(ItemInfo);
    item->Description = GET_SINGLE(ItemManager)->GetDescription(ItemInfo);
    item->Price = GET_SINGLE(ItemManager)->GetPrice(ItemInfo);
    item->Type = GET_SINGLE(ItemManager)->GetType(ItemInfo);
    item->SubType = GET_SINGLE(ItemManager)->GetSubType(ItemInfo);
    item->Sprite = GET_SINGLE(ItemManager)->GetSprite(item->Name);

    // 동적으로 할당한 ITEM 객체의 참조를 반환
    return *item;
}

bool ItemManager::AddItemToInventory(int itemId)
{
    return GetInventory()->AddItem(itemId);
}

bool ItemManager::AddItemToInventory(int itemId, int counts)
{
    if (itemId == 0)
        return false;

    return GetInventory()->AddItem(itemId, counts);
}

bool ItemManager::RemoveItemFromInventory(int itemId)
{
    return GetInventory()->RemoveItem(itemId);
}

void ItemManager::SetItemToQuickSlot(ITEM* item, int index)
{
    GetQuickSlot()->SetQuickSlot(item, index);
}

int ItemManager::GetQuickSlotSelectedIndex()
{
    return _quickSlot->GetSelectedIndex();
}

wstring ItemManager::GetQuickSlotSelectedSubType()
{
    return _quickSlot->GetSelectedSubType();
}

void ItemManager::EquipItem(ITEM& item)
{
    _inventory->EquipItem(item);
}

bool ItemManager::IsInventoryFull()
{
    auto slots = _inventory->GetSlots();

    for (auto& slot : slots)
    {
        if (slot.ItemId == 0)
            return false;
    }

    return true;
}

int ItemManager::GetEmptySlots()
{
    auto slots = _inventory->GetSlots();
    int emptySlots = 0;

    for (auto& slot : slots)
    {
        if (slot.ItemId == 0)
            emptySlots++;
    }

    return emptySlots;
}

wstring ItemManager::GetName(vector<wstring> row)
{
    return row[1];
}

wstring ItemManager::GetKorName(vector<wstring> row)
{
    return row[2];
}

wstring ItemManager::GetType(vector<wstring> row)
{
    return row[3];
}

wstring ItemManager::GetSubType(vector<wstring> row)
{
    return row[4];
}

wstring ItemManager::GetDescription(vector<wstring> row)
{
    return row[5];
}

int ItemManager::GetPrice(vector<wstring> row)
{
    return stoi(row[6]);
}

Sprite* ItemManager::GetSprite(wstring wstr)
{
    return GET_SINGLE(ResourceManager)->GetSprite(wstr);
}