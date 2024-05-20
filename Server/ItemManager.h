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
	// �׷� ���̺�
	int GetMonsterType(vector<wstring> row) { return stoi(row[0]); }
	int GetWearableProbablity(vector<wstring> row) { return stoi(row[1]); }
	int GetConsumableProbablity(vector<wstring> row) { return stoi(row[2]); }
	int GetETCProbablity(vector<wstring> row) { return stoi(row[3]); }
	int GetNoneProbablity(vector<wstring> row) { return stoi(row[4]); }
	vector<wstring> GetMonsterDropItemGroupInfo(Protocol::MONSTER_TYPE monsterType);

public:
	// ������ ��� ���̺�
	wstring GetDropItemGroup(vector<wstring> row) { return row[1]; }
	int GetDropItemID(vector<wstring> row) { return stoi(row[2]); }
	wstring GetDropItemName(vector<wstring> row) { return row[3]; }
	int GetDropItemProbablity(vector<wstring> row) { return stoi(row[4]); }
	vector<wstring> GetMonsterDropItemInfo(Protocol::MONSTER_TYPE monsterType);

public:
	// ������ ��� ���̺� ��ȸ �Լ�
	vector<vector<wstring>> GetItemDropTableByMonsterType(Protocol::MONSTER_TYPE monsterType);
	vector<vector<wstring>> GetItemDropTableByGroup(vector<vector<wstring>> dropTable, wstring itemGroup);
	vector<vector<wstring>> GetItemDropTable() { return _itemDropTable; }
	vector<vector<wstring>> GetItemGroupTable() { return _itemGroupTable; }

	int CalculateItemTotalProbablity(vector<vector<wstring>> dropTable);
	int GetItemFromGroup(vector<vector<wstring>> dropTable);
	void MakeItem(vector<vector<wstring>> group, CreatureRef owner, Vec2Int pos);

public:
	// ������ ����
	vector<wstring> GetItemInfo(int itemID);
	string GetItemNameByID(int itemID);
	Protocol::ITEM_TYPE GetItemTypeByID(int itemID);
	string GetItemSubTypeByID(int itemID);


private:
	vector<vector<wstring>> _itemGroupTable;
	vector<vector<wstring>> _itemDropTable;
	vector<vector<wstring>> _itemTable;
};

