#include "pch.h"
#include "Quest.h"
#include "GameRoom.h"

#include <fstream>
#include <sstream>


Quest::~Quest()
{
}

void Quest::Init()
{
	_questTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Server\\QuestTable.csv");
}

void Quest::Accept()
{
}

void Quest::Completed()
{
}

vector<vector<wstring>> Quest::GetDataFromCSV(const string& filename)
{
	vector<vector<wstring>> data;
	locale::global(locale("korean"));

	wifstream file(filename);
	if (!file.is_open())
	{
		return data;
	}

	// 첫 번째 행을 건너뛰기 위해 한 번 더 getline 호출
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

vector<wstring> Quest::GetQuestInfo(int questID)
{
	for (auto& row : _questTable)
	{
		if (row.empty())
			return {};

		int result;
		std::wistringstream(row[0]) >> result;

		if (result == questID)
		{
			return row;
		}
	}

	// 실패하면 무엇을 반환할까
	return {};
}

void Quest::CreateQuest()
{
	Quest* quest = new Quest();

	for (auto& questTable : _questTable)
	{
		int Questid = stoi(questTable[0]);
		int TargetID = stoi(questTable[1]);
		wstring TargetType = questTable[2];
		int TargetCounts = stoi(questTable[3]);
		int Gold = stoi(questTable[4]);
		int ItemID = stoi(questTable[5]);
		int ItemCounts = stoi(questTable[6]);

		quest->info.set_questid(Questid);
		quest->info.set_targetid(TargetID);
		quest->info.set_queststate(Protocol::QUEST_STATE_IDLE);

		if (TargetType == L"None")
		{
			quest->info.set_targettype(Protocol::OBJECT_TYPE_NONE);
		}
		else if (TargetType == L"Monster")
		{
			quest->info.set_targettype(Protocol::OBJECT_TYPE_MONSTER);
		}
		else if (TargetType == L"Item")
		{
			quest->info.set_targettype(Protocol::OBJECT_TYPE_ITEM);
		}

		quest->info.set_targetnums(TargetCounts);
		quest->info.set_rewardgold(Gold);
		quest->info.set_rewarditem(ItemID);
		quest->info.set_rewarditemnum(ItemCounts);

		GRoom->AddQuest(*quest);
	}
}