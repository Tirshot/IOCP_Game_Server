#include "pch.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "QuestTrackerUI.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

QuestManager::~QuestManager()
{
    _quests.clear();
}

void QuestManager::BeginPlay()
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		_tracker = scene->FindUI<QuestTrackerUI>(scene->GetUIs());
	}
}
float sumTime = 0.f;
void QuestManager::Tick()
{
    // 0.1초 마다 실행
    float now = GET_SINGLE(TimeManager)->GetDeltaTime();

    sumTime += now;

    if (sumTime <= 0.1f)
        return;

    sumTime = 0.f;
    
    // 필수 객체 가져오기
    auto scene = GET_SINGLE(SceneManager)->GetDevScene();
    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    auto inventory = GET_SINGLE(ItemManager)->GetInventory();
    
    if (!myPlayer || !scene || !inventory)
        return;

    auto& questStates = myPlayer->GetQuestStates();
    if (questStates.empty())
        return;

    for (auto& questState : questStates)
    {
        int questID = questState.first;
        int progress = questState.second.second;
        int state = questState.second.first;

        auto& info = scene->GetQuest(questID);
        int targetType = info.targettype();
        int targetNums = info.targetnums();
        int targetID = info.targetid();
        auto questScriptInfo = GetQuestScriptInfo(questID);
        wstring questName = questScriptInfo.questName;
        wstring description = questScriptInfo.description;

        // 트래커 진행도 갱신
        _tracker->SetProgress(questID, progress);

        // 퀘스트가 트래커에 추가되지 않았으면 추가
        if (state == Protocol::QUEST_STATE_ACCEPT ||
            state == Protocol::QUEST_STATE_COMPLETED)
        {
            if (!_tracker->IsQuestInTracker(questID))
                _tracker->AddQuestToTracking(questID, questName, description, targetNums);
        }

        bool questComplete = false;

        // 퀘스트 상태 처리
        switch (targetType)
        {
        case Protocol::OBJECT_TYPE_ITEM:
        {
            auto item = GET_SINGLE(ItemManager)->FindItemFromInventory(targetID);
            int itemCount = item ? item->ItemCount : 0;

            if (state == Protocol::QUEST_STATE_ACCEPT)
            {
                _tracker->SetProgress(questID, itemCount);
                if (itemCount >= targetNums)
                {
                    myPlayer->SetQuestState(questID, Protocol::QUEST_STATE_COMPLETED);
                    questComplete = true;
                    continue;
                }
            }
            else if (state == Protocol::QUEST_STATE_COMPLETED)
            {
                _tracker->SetProgress(questID, itemCount);

                if (_announce == false)
                {
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" QUEST COMPLETE!!");
                    GET_SINGLE(ChatManager)->AddMessage(L"상인에게 돌아가서 보상을 받으세요.");
                    GET_SINGLE(SoundManager)->Play(L"QuestComplete");
                    _announce = true;
                    continue;
                }
            }
            else if (state == Protocol::QUEST_STATE_FINISHED)
            {
                if (_announce == false)
                {
                    inventory->RemoveItem(targetID, targetNums);
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" 퀘스트 완료.");
                    _tracker->RemoveQuestFromTracker(questID);
                    _announce = true;
                    continue;
                }
            }
            break;
        }
        case Protocol::OBJECT_TYPE_MONSTER:
        {
            if (state == Protocol::QUEST_STATE_ACCEPT)
            {
                if (progress == targetNums)
                {
                    myPlayer->SetQuestState(questID, Protocol::QUEST_STATE_COMPLETED);
                    questComplete = true;
                    continue;
                }
            }
            else if (state == Protocol::QUEST_STATE_COMPLETED)
            {
                if (_announce == false)
                {
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" QUEST COMPLETE!!");
                    GET_SINGLE(ChatManager)->AddMessage(L"상인에게 돌아가서 보상을 받으세요.");
                    GET_SINGLE(SoundManager)->Play(L"QuestComplete");
                    _announce = true;
                    continue;
                }
            }
            else if (state == Protocol::QUEST_STATE_FINISHED)
            {
                _tracker->RemoveQuestFromTracker(questID);
                if (_announce == false)
                {
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" 퀘스트 완료.");
                    _announce = true;
                    break;
                }
            }
            break;
        }
        }

        if (questComplete && _announce == false)
        {
            GET_SINGLE(ChatManager)->AddMessage(questName + L" 퀘스트 진행중.");
            _announce = true;
            return;
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

	if (questTable.empty())
		return {};

	vector<wstring> questInfo = questTable[questID];
	if (questInfo.empty())
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
