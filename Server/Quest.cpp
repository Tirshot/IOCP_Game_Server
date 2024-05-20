#include "pch.h"
#include "Quest.h"
#include "GameRoom.h"
#include "Chat.h"
#include "Player.h"
#include "GameSession.h"
#include <fstream>
#include <sstream>


Quest::~Quest()
{
}

void Quest::Init()
{
	_questTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Server\\QuestTable.csv");
}

void Quest::Update()
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

	return {};
}

vector<wstring> Quest::GetQuestInfoByTargetID(wstring targetType, int targetID)
{
	for (auto& row : _questTable)
	{
		if (row.empty())
			return {};

		int id;
		wstring type;
		std::wistringstream(row[2]) >> type;
		std::wistringstream(row[1]) >> id;

		if (type == targetType)
		{
			if (id == targetID)
			{
				return row;
			}
			continue;
		}
	}

	return {};
}

void Quest::ItemQuestCheck(int objectID, int itemID)
{
	// 타겟 타입이 아이템인지 체크
	// 타겟 ID와 아이템 ID가 같은지 체크
	auto questInfo = GET_SINGLE(Quest)->GetQuestInfoByTargetID(L"Item", itemID);

	if (questInfo.empty())
		return;

	auto questTargetCounts = GET_SINGLE(Quest)->GetTargetCounts(questInfo);
	auto questTargetID = GET_SINGLE(Quest)->GetTargetID(questInfo);

	int questID = GET_SINGLE(Quest)->GetID(questInfo);

	PlayerRef player = dynamic_pointer_cast<Player>(GRoom->FindObject(objectID));
	if (player)
	{
		auto questState = player->GetQuestState(questID);
		auto questProgressCounts = player->GetQuestProgress(questID);

		if (questState.first == Protocol::QUEST_STATE_ACCEPT)
		{
			if (questTargetID == itemID)
				player->SetQuestProgress(questID, questProgressCounts + 1);
		}

		if (questProgressCounts == questTargetCounts)
		{
			player->SetQuestState(questID, Protocol::QUEST_STATE_COMPLETED);

			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_QuestComplete(objectID, questID, questTargetCounts);
			player->session->Send(sendBuffer);
		}
	}
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