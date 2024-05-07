#include "pch.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

QuestManager::~QuestManager()
{
}

void QuestManager::BeginPlay()
{
}

void QuestManager::Tick()
{
	// 퀘스트 조건 확인
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer && scene)
	{
		auto& questStates = myPlayer->GetQuestStates();

		if (questStates.size() == 0)
			return;

		for (auto& questState : questStates)
		{
			int questID = questState.first;
			int progress = questState.second.second;
			int state = questState.second.first;

			if (state == Protocol::QUEST_STATE_ACCEPT)
			{
				auto& info = scene->GetQuest(questID);
				auto targetType = info.targettype();
				int targetNums = info.targetnums();
				int targetID = info.targetid();

				switch (targetType)
				{
				case Protocol::OBJECT_TYPE_ITEM:
				{
					ITEM& item = *GET_SINGLE(ItemManager)->FindItemFromInventory(targetID);
					int itemCount = item.ItemCount;
					if (itemCount == targetNums)
					{
						// 퀘스트 완료

						// 아이템 제거
					}
					else
					{
						// 퀘스트 진행 중, 트래킹

					}
					break;
				}

				case Protocol::OBJECT_TYPE_MONSTER:
				{
					// 몬스터 처리 기록
					int progress = myPlayer->GetQuestProgress(questID);
					if (progress == targetNums)
					{
						// 퀘스트 완료

					}
					else
					{
						// 퀘스트 진행 중, 트래킹

					}
					break;
				}
				}
			}

		}
	}
}

QuestInfo QuestManager::GetQuestScriptInfo(int questID)
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
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayerId();
	auto questTable = GET_SINGLE(ResourceManager)->GetQuestTable();

	if (questTable.size() < questID)
		return {};

	vector<wstring> questInfo = questTable[questID];
	if (questInfo.size() == 0)
		return {};

	// 퀘스트 진행도에 따라 스크립트를 불러오기
	if (scene && myPlayer)
	{
		wstring questScript = L"";
		auto questState = scene->GetPlayerQuestState(myPlayer, questID);

		switch (questState)
		{
			// 퀘스트 미진행
		case Protocol::QUEST_STATE_IDLE:
			questScript = questInfo[4];
			break;

			// 퀘스트 수락 이후
		case Protocol::QUEST_STATE_ACCEPT:
			questScript = questInfo[5];
			break;

			// 퀘스트 완료
		case Protocol::QUEST_STATE_COMPLETED:
			questScript = questInfo[6];
			break;

		default:
			questScript = questInfo[4];
			break;
		}

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
	return {};
}
