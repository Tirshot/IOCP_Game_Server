#pragma once
class ItemManager
{
	DECLARE_SINGLE(ItemManager);

public:
	virtual void Init();
	virtual void Tick();

	vector<vector<wstring>> GetDataFromCSV(const string& filename);
	string WStringToString(const wstring& wstr);
	wstring StringToWString(const string& str);

public:
	// 그룹 테이블
	int GetMonsterType(vector<wstring> row) { return stoi(row[0]); }
	int GetWearableProbablity(vector<wstring> row) { return stoi(row[1]); }
	int GetConsumableProbablity(vector<wstring> row) { return stoi(row[2]); }
	int GetETCProbablity(vector<wstring> row) { return stoi(row[3]); }
	int GetNoneProbablity(vector<wstring> row) { return stoi(row[4]); }
	int GetMinGold(vector <wstring> row) { return stoi(row[5]); }
	int GetMaxGold(vector <wstring> row) { return stoi(row[6]); }
	vector<wstring> GetMonsterDropItemGroupInfo(Protocol::MONSTER_TYPE monsterType);

public:
	// 아이템 드랍 테이블
	wstring GetDropItemGroup(vector<wstring> row) { return row[1]; }
	int GetDropItemID(vector<wstring> row) { return stoi(row[2]); }
	wstring GetDropItemName(vector<wstring> row) { return row[3]; }
	int GetDropItemProbablity(vector<wstring> row) { return stoi(row[4]); }
	int GetDropItemRemoveTime(vector<wstring> row) { return stoi(row[5]); }
	vector<wstring> GetMonsterDropItemInfo(Protocol::MONSTER_TYPE monsterType);

public:
	// 아이템 드랍 테이블 순회 함수
	vector<vector<wstring>> GetItemDropTableByMonsterType(Protocol::MONSTER_TYPE monsterType);
	vector<vector<wstring>> GetItemDropTableByGroup(vector<vector<wstring>> dropTable, wstring itemGroup);
	vector<vector<wstring>> GetItemDropTable() { return _itemDropTable; }
	vector<vector<wstring>> GetItemGroupTable() { return _itemGroupTable; }

	int CalculateItemTotalProbablity(vector<vector<wstring>> dropTable);
	int GetItemFromGroup(vector<vector<wstring>> dropTable);
	int GetItemRemoveTimeFromGroup(vector<vector<wstring>> dropTable, int itemID);
	shared_ptr<class Item> MakeItem(vector<vector<wstring>> group, CreatureRef owner, Vec2Int pos);

public:
	// 아이템 정보
	vector<wstring> GetItemInfo(int itemID);
	string GetItemNameByID(int itemID);
	Protocol::ITEM_TYPE GetItemTypeByID(int itemID);
	Protocol::ITEM_SUBTYPE GetItemSubType(int itemID);
	string GetItemSubTypeByID(int itemID);
	int GetItemRemoveTimeByID(int itemID);


private:
	vector<vector<wstring>> _itemGroupTable;
	vector<vector<wstring>> _itemDropTable;
	vector<vector<wstring>> _itemTable;
};

