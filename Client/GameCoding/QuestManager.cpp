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
    // 0.1�� ���� ����
    float now = GET_SINGLE(TimeManager)->GetDeltaTime();

    sumTime += now;

    if (sumTime <= 0.1f)
        return;

    sumTime = 0.f;
    
    // �ʼ� ��ü ��������
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

        // Ʈ��Ŀ ���൵ ����
        _tracker->SetProgress(questID, progress);

        // ����Ʈ�� Ʈ��Ŀ�� �߰����� �ʾ����� �߰�
        if (state == Protocol::QUEST_STATE_ACCEPT ||
            state == Protocol::QUEST_STATE_COMPLETED)
        {
            if (!_tracker->IsQuestInTracker(questID))
                _tracker->AddQuestToTracking(questID, questName, description, targetNums);
        }

        bool questComplete = false;

        // ����Ʈ ���� ó��
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
                    GET_SINGLE(ChatManager)->AddMessage(L"���ο��� ���ư��� ������ ��������.");
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
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" ����Ʈ �Ϸ�.");
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
                    GET_SINGLE(ChatManager)->AddMessage(L"���ο��� ���ư��� ������ ��������.");
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
                    GET_SINGLE(ChatManager)->AddMessage(questName + L" ����Ʈ �Ϸ�.");
                    _announce = true;
                    break;
                }
            }
            break;
        }
        }

        if (questComplete && _announce == false)
        {
            GET_SINGLE(ChatManager)->AddMessage(questName + L" ����Ʈ ������.");
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

	// ����Ʈ ���൵�� ���� ��ũ��Ʈ�� �ҷ�����
	if (scene && myPlayer)
	{
		wstring questScript = L"";
		auto questState = scene->GetPlayerQuestState(myPlayer, questID);

		switch (questState)
		{
			// ����Ʈ ������
		case Protocol::QUEST_STATE_IDLE:
			questScript = questInfo[4];
			break;

			// ����Ʈ ���� ����
		case Protocol::QUEST_STATE_ACCEPT:
			questScript = questInfo[5];
			break;

			// ����Ʈ �Ϸ�
		case Protocol::QUEST_STATE_COMPLETED:
			questScript = questInfo[6];
			break;

		default:
			questScript = questInfo[4];
			break;
		}

		// �ٹٲ� ���ڸ� �������� ���ڿ� ����
		vector<wstring> lines;
		size_t pos = 0;
		while ((pos = questScript.find(L"\\")) != wstring::npos)
		{
			lines.push_back(questScript.substr(0, pos));
			questScript.erase(0, pos + 1);
		}
		lines.push_back(questScript); // ������ �� �߰�

		return lines;
	}
	return {};
}
