#include "pch.h"
#include "ItemManager.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "QuickSlot.h"

void ItemManager::Init()
{
	_itemTable = GET_SINGLE(FileManager)->GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Client\\Resources\\Table\\ItemTable.csv");
    
    DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
    if (scene == nullptr)
        return;

    _inventory = scene->FindUI<Inventory>(scene->GetUIs());
    _quickSlot = scene->FindUI<QuickSlot>(scene->GetUIs());
}

vector<wstring> ItemManager::FindItemInfo(int itemID)
{
    for (auto& row : _itemTable)
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
    for (auto& row : _itemTable)
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

bool ItemManager::AddItemToInventory(int itemId)
{
    return GetInventory()->AddItem(itemId);
}

bool ItemManager::RemoveItemFromInventory(int itemId)
{
    return GetInventory()->RemoveItem(itemId);
}

void ItemManager::SetItemToQuickSlot(ITEM* item, int index)
{
    GetQuickSlot()->SetQuickSlot(item, index);
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

Sprite* ItemManager::GetSprite(wstring wstr)
{
    return GET_SINGLE(ResourceManager)->GetSprite(wstr);
}