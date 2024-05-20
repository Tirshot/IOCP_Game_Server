#pragma once
class Quest
{
	DECLARE_SINGLE(Quest);

public:
	virtual ~Quest();

	virtual void Init();
	virtual void Update();
	virtual void Completed();

public:
	vector<vector<wstring>> GetDataFromCSV(const string& filename);
	int GetID(vector<wstring> row) { return stoi(row[0]); }
	int GetTargetID(vector<wstring> row) { return stoi(row[1]); }
	wstring GetTargetType(vector<wstring> row) { return row[2]; }
	int GetTargetCounts(vector<wstring> row) { return stoi(row[3]); }
	int GetRewardGold(vector<wstring> row) { return stoi(row[4]); }
	int GetRewardItemID(vector<wstring> row) { if (row[5] != L"") return stoi(row[5]); else return 0; }
	int GetRewardItemCounts(vector<wstring> row) { if (row[6] != L"") return stoi(row[6]); else return 0; }
	vector<wstring> GetQuestInfo(int questID);
	vector<wstring> GetQuestInfoByTargetID(wstring targetType, int targetID);


public:
	void CreateQuest();
	void ItemQuestCheck(int objectID, int itemID);

public:
	Protocol::QuestInfo info;
	vector<vector<wstring>> _questTable;

};