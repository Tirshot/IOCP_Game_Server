#include "pch.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "QuickSlot.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

void ItemManager::Init()
{
    auto scene = GET_SINGLE(SceneManager)->GetDevScene();
    if (scene == nullptr)
        return;

    _inventory = scene->FindUI<Inventory>(scene->GetUIs());
    _quickSlot = scene->FindUI<QuickSlot>(scene->GetUIs());
}

void ItemManager::Clear()
{
}

void ItemManager::Tick()
{

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

ITEM ItemManager::GetItem(int itemID)
{
    // 아이템 정보를 찾음
    vector<wstring> ItemInfo = FindItemInfo(itemID);

    // ITEM 객체를 동적으로 할당
    auto item = make_shared<ITEM>();

    // 아이템 정보 할당
    item->ItemId = itemID;
    item->ItemCount = 1;
    item->Name = GetName(ItemInfo);
    item->KorName = GetKorName(ItemInfo);
    item->Description = GetDescription(ItemInfo);
    item->Price = GetPrice(ItemInfo);
    item->Type = GetType(ItemInfo);
    item->SubType = GetSubType(ItemInfo);
    item->Sprite = GetSprite(item->Name);

    // 동적으로 할당한 ITEM 객체의 참조를 반환
    return *item;
}

bool ItemManager::AddItemToInventory(int itemId)
{
    return GetInventory()->AddItem(itemId);
}

bool ItemManager::AddItemToInventory(int itemId, int counts)
{
    if (itemId <= 0)
        return false;

    return GetInventory()->AddItem(itemId, counts);
}

bool ItemManager::SetItemToInventory(int itemId, int counts)
{
    if (itemId <= 0)
        return false;

    GetInventory()->SetItemCount(itemId, counts);

    return true;
}

bool ItemManager::RemoveItemFromInventory(int itemId)
{
    return GetInventory()->RemoveItem(itemId);
}

void ItemManager::SetItemToQuickSlot(shared_ptr<ITEM> item, int index)
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

void ItemManager::EquipItem(shared_ptr<ITEM> item)
{
    _inventory->EquipItem(item);
}

void ItemManager::QuickEquipItem(int itemID)
{
    _inventory->QuickEquipItem(itemID);
}

bool ItemManager::IsInventoryFull()
{
    auto slots = _inventory->GetSlots();

    for (auto& slot : slots)
    {
        if (slot->ItemId == 0)
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
        if (slot->ItemId == 0)
            emptySlots++;
    }

    return emptySlots;
}

shared_ptr<ITEM> ItemManager::FindItemFromInventory(int itemId)
{
    return _inventory->FindItemFromInventory(itemId);
}

void ItemManager::SyncToServer()
{
    auto slots = _inventory->GetSlots();

    for (auto& slot : slots)
    {
        if (slot->ItemId != 0)
        {
            slot->ItemId;
            slot->ItemCount;
        }
    }

}

void ItemManager::OpenInventory()
{
    if (_inventory->GetVisible())
    {
        _inventory->SetVisible(false);
    }
    else
    {
        _inventory->SetVisible(true);
    }
}

void ItemManager::ResetInventory()
{
    // 부활 했을 경우에만 사용
    // 초기화된 인벤토리를 지우고 서버에서 받을 것
    _inventory->ResetInventory();
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
    if (!row.empty())
        return row[3];

    return L"";
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

shared_ptr<Sprite> ItemManager::GetSprite(wstring wstr)
{
    return GET_SINGLE(ResourceManager)->GetSprite(wstr);
}

shared_ptr<Sprite> ItemManager::GetSprite(int itemID)
{
    const auto ItemTable = GET_SINGLE(ResourceManager)->GetItemTable();

    for (auto& row : ItemTable)
    {
        if (row.empty())
            return nullptr;

        if (stoi(row[0]) == itemID)
        {
            return GET_SINGLE(ResourceManager)->GetSprite(row[1]);
        }
    }

}
