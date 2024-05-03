#include "pch.h"
#include "QuestManager.h"
#include "ResourceManager.h"

QuestManager::~QuestManager()
{
}

void QuestManager::BeginPlay()
{
}

void QuestManager::Tick()
{
}

QuestInfo QuestManager::GetQuestInfo(int questID)
{
	auto questTable =  GET_SINGLE(ResourceManager)->GetQuestTable();

	if (questTable.size() < questID)
		return {};

	vector<wstring> questInfo = questTable[questID];
	if (questInfo.size() == 0)
		return {};

	wstring questName = questInfo[1];
	wstring questNPC = questInfo[2];
	wstring description = questInfo[3];
	wstring questScript = questInfo[4];

	QuestInfo info = {};
	info.questID = questID;
	info.questNPC = questNPC;
	info.questName = questName;
	info.description = description;
	info.questScript = questScript;

	return info;
}

vector<wstring> QuestManager::GetQuestScript(int questID)
{
	auto questTable = GET_SINGLE(ResourceManager)->GetQuestTable();

	if (questTable.size() < questID)
		return {};

	vector<wstring> questInfo = questTable[questID];
	if (questInfo.size() == 0)
		return {};

	wstring questScript = questInfo[4];

	// 줄바꿈 문자를 기준으로 문자열 분할
	vector<wstring> lines;
	size_t pos = 0;
	while ((pos = questScript.find(L"\\")) != wstring::npos)
	{
		lines.push_back(questScript.substr(0, pos));
		questScript.erase(0, pos + 1);
	}
	lines.push_back(questScript); // 마지막 줄 추가

	return lines;
}
