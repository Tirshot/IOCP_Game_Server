#include "pch.h"
#include "ItemManager.h"
#include "Item.h"
#include "Creature.h"
#include "Player.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Chat.h"
#include <codecvt>

#include <fstream>
#include <sstream>

void ItemManager::Init()
{
	_itemGroupTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Server\\ItemGroupDropTable.csv");
	_itemDropTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Server\\ItemDropTable.csv");
	_itemTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Server\\ItemTable.csv");
}

void ItemManager::Tick()
{

}

vector<vector<wstring>> ItemManager::GetDataFromCSV(const string& filename)
{
	vector<vector<wstring>> data;
	locale::global(locale("korean"));

	wifstream file(filename);
	if (!file.is_open())
	{
		return data;
	}

	// ù ��° ���� �ǳʶٱ� ���� �� �� �� getline ȣ��
	wstring header;
	getline(file, header);

	wstring line;
	while (getline(file, line))
	{
		vector<wstring> row;
		wstringstream ss(line);
		wstring cell;

		while (getline(ss, cell, L','))
		{
			row.push_back(cell);
		}
		data.push_back(row);
	}

	file.close();

	return data;
}

string ItemManager::WStringToString(const wstring& wstr)
{
	// wstring to string ��ȯ
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	string str2(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

	return str2;
}

wstring ItemManager::StringToWString(const string& str)
{
	// string to wstring ��ȯ
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

	wstring str2(bufferSize, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize);

	return str2;
}

vector<wstring> ItemManager::GetMonsterDropItemGroupInfo(Protocol::MONSTER_TYPE monsterType)
{
	for (auto& row : _itemGroupTable)
	{
		if (row.empty())
			return {};

		int type;
		std::wistringstream(row[0]) >> type;

		if (type == monsterType)
		{
			return row;
		}
	}

	return {};
}

vector<wstring> ItemManager::GetMonsterDropItemInfo(Protocol::MONSTER_TYPE monsterType)
{
	for (auto& row : _itemDropTable)
	{
		if (row.empty())
			return {};

		int type;
		std::wistringstream(row[0]) >> type;

		if (type == monsterType)
		{
			return row;
		}
	}

	return {};
}

vector<vector<wstring>> ItemManager::GetItemDropTableByMonsterType(Protocol::MONSTER_TYPE monsterType)
{
	vector<vector<wstring>> ItemDropTable;

	for (auto& row : _itemDropTable)
	{
		// �� ���̸� ����
		if (row.empty())
			break;

		int type = 0;
		std::wistringstream(row[0]) >> type;

		if (type == monsterType)
		{
			ItemDropTable.push_back(row);
		}
		else
		{
			continue;
		}
	}

	return ItemDropTable;
}

vector<vector<wstring>> ItemManager::GetItemDropTableByGroup(vector<vector<wstring>> dropTable, wstring itemGroup)
{
	vector<vector<wstring>> ItemDropTable;

	for (auto& row : dropTable)
	{
		// �� ���̸� ����
		if (row.empty())
			break;

		wstring group = L"";
		std::wistringstream(row[1]) >> group;

		if (group == itemGroup)
		{
			ItemDropTable.push_back(row);
		}
		else
		{
			continue;
		}
	}

	return ItemDropTable;
}

int ItemManager::CalculateItemTotalProbablity(vector<vector<wstring>> dropTable)
{
	int probablity = 0;

	for (auto& row : dropTable)
	{
		// �� ���̸� ����
		if (row.empty())
			break;

		probablity += GetDropItemProbablity(row);
	}

	return probablity;
}

int ItemManager::GetItemFromGroup(vector<vector<wstring>> dropTable)
{
	// ���̺��� �� Ȯ�� ���
	int total = GET_SINGLE(ItemManager)->CalculateItemTotalProbablity(dropTable);

	if (total <= 0)
	{
		GChat->AddText(::format(L"<ERROR> ��� ���̺� �� Ȯ���� 0���� ����."));
		return 0;
	}

	// �ֻ��� ������
	int randItemValue = rand() % total + 1;

	int dropValue = 0;

	for (int i = 0; i < dropTable.size(); i++)
	{
		// �׷� Ȯ�� ����
		dropValue += GetDropItemProbablity(dropTable[i]);

		if (randItemValue <= dropValue)
		{
			// ��÷
			return GetDropItemID(dropTable[i]);
		}
	}

	assert(false);
	return -1;  // ���� ó��
}

int ItemManager::GetItemRemoveTimeFromGroup(vector<vector<wstring>> dropTable, int itemID)
{
	for (const auto& row : dropTable)
	{
		if (stoi(row[2]) == itemID)
			return stoi(row[5]);
	}

	assert(false);
	return -1;  // ���� ó��
}

shared_ptr<Item> ItemManager::MakeItem(vector<vector<wstring>> group, CreatureRef owner, Vec2Int pos)
{
	int itemID = GetItemFromGroup(group);
	string name = GET_SINGLE(ItemManager)->GetItemNameByID(itemID);
	auto type = GET_SINGLE(ItemManager)->GetItemTypeByID(itemID);
	auto subType = GET_SINGLE(ItemManager)->GetItemSubTypeByID(itemID);
	wstring wname = GET_SINGLE(ItemManager)->StringToWString(name);
	auto removeTime = GET_SINGLE(ItemManager)->GetItemRemoveTimeFromGroup(group, itemID);

	if (GRoom)
	{
		ItemRef item1 = GameObject::CreateItem();
		auto itemObjectID = item1->GetObjectID();
		item1->info.set_posx(pos.x);
		item1->info.set_posy(pos.y);
		item1->info.set_defence(9999);
		item1->info.set_name(name);
		item1->info.set_itemtype(type);
		item1->SetOwner(owner->GetObjectID());

		GRoom->AddObject(item1);
		{
			auto& itemInfo = item1->itemInfo;
			itemInfo.set_objectid(itemObjectID);
			itemInfo.set_posx(pos.x);
			itemInfo.set_posy(pos.y);
			itemInfo.set_itemid(itemID);
			itemInfo.set_itemcount(1);
			itemInfo.set_itemtype(type);
			itemInfo.set_itemname(name);
			itemInfo.set_removetime(removeTime);

			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_ItemDrop(itemInfo);
			PlayerRef player = static_pointer_cast<Player>(GRoom->FindObject(owner->GetObjectID()));

			if (player)
			{
				player->session->Send(sendBuffer);
				GChat->AddText(format(L"������ Player{0}, [{1}, {2}] ��ġ�� {3} ������ ���", owner->GetObjectID(), pos.x, pos.y, wname));
			}
		}
		item1->BeginPlay();
		return item1;
	}
}

vector<wstring> ItemManager::GetItemInfo(int itemID)
{
	for (auto& row : _itemTable)
	{
		if (row.empty())
			return {};

		int itemId;
		std::wistringstream(row[0]) >> itemId;

		if (itemId == itemID)
		{
			return row;
		}
	}

	return {};
}

string ItemManager::GetItemNameByID(int itemID)
{
	auto info = GetItemInfo(itemID);

	return WStringToString(info[1]);
}

Protocol::ITEM_TYPE ItemManager::GetItemTypeByID(int itemID)
{
	auto str = GetItemNameByID(itemID);

	if (str == "Wearable")
	{
		return Protocol::ITEM_TYPE_WEARABLE;
	}
	else if (str == "Consumable")
	{
		return Protocol::ITEM_TYPE_CONSUMABLE;
	}
	else if (str == "ETC")
	{
		return Protocol::ITEM_TYPE_ETC;
	}
	else
	{
		return Protocol::ITEM_TYPE_NONE;
	}
}

Protocol::ITEM_SUBTYPE ItemManager::GetItemSubType(int itemID)
{
	auto info = GetItemInfo(itemID);

	wstring subType = info[4];

	if (subType == L"Helmet")
	{
		return Protocol::WEARABLE_TYPE_HELMET;
	}
	else if (subType == L"Armor")
	{
		return Protocol::WEARABLE_TYPE_ARMOR;
	}
	else if (subType == L"Pants")
	{
		return Protocol::WEARABLE_TYPE_PANTS;
	}
	else if (subType == L"Boots")
	{
		return Protocol::WEARABLE_TYPE_BOOTS;
	}
}

string ItemManager::GetItemSubTypeByID(int itemID)
{
	auto info = GetItemInfo(itemID);

	return WStringToString(info[4]);
}

int ItemManager::GetItemRemoveTimeByID(int itemID)
{
	auto info = GetItemInfo(itemID);

	return GetDropItemRemoveTime(info);
}
