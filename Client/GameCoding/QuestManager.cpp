#include "pch.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "SoundManager.h"

QuestManager::~QuestManager()
{
}

void QuestManager::BeginPlay()
{
}

void QuestManager::Tick()
{
//	// ����Ʈ ���� Ȯ��
//	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
//	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
//	auto inventory = GET_SINGLE(ItemManager)->GetInventory();
//
//	if (myPlayer && scene && inventory)
//	{
//		auto& questStates = myPlayer->GetQuestStates();
//
//		if (questStates.size() == 0)
//			return;
//
//		for (auto& questState : questStates)
//		{
//			int questID = questState.first;
//			int progress = questState.second.second;
//			int state = questState.second.first;
//
//			if (state == Protocol::QUEST_STATE_ACCEPT)
//			{
//				auto& info = scene->GetQuest(questID);
//				auto targetType = info.targettype();
//				int targetNums = info.targetnums();
//				int targetID = info.targetid();
//
//				switch (targetType)
//				{
//				case Protocol::OBJECT_TYPE_ITEM:
//				{
//					ITEM& item = *GET_SINGLE(ItemManager)->FindItemFromInventory(targetID);
//					int itemCount = item.ItemCount;
//					if (itemCount >= targetNums)
//					{
//						// ����Ʈ �Ϸ�
//						myPlayer->SetQuestState(questID, Protocol::QUEST_STATE_COMPLETED);
//
//						if (_announce == false)
//						{
//							wstring questName = GetQuestScriptInfo(questID).questName;
//							GET_SINGLE(ChatManager)->AddMessage(questName + L" QUEST COMPLETE!!");
//							GET_SINGLE(ChatManager)->AddMessage(L"���ο��� ���ư��� ������ ��������.");
//							GET_SINGLE(SoundManager)->Play(L"QuestComplete");
//							_announce = true;
//						}
//
//						// ������ �����鼭 ������ ����
//						if (myPlayer->GetQuestState(questID) == Protocol::QUEST_STATE_FINISHED)
//						{
//							inventory->RemoveItem(item.ItemId, targetNums);
//							_announce = false;
//							return;
//						}
//					}
//					else
//					{
//						if (_announce == false)
//						{
//							// ����Ʈ ���� ��, Ʈ��ŷ
//							wstring questName = GetQuestScriptInfo(questID).questName;
//							GET_SINGLE(ChatManager)->AddMessage(questName + L" ����Ʈ ������.");
//							_announce = true;
//						}
//					}
//					break;
//				}
//
//				case Protocol::OBJECT_TYPE_MONSTER:
//				{
//					// ���� ó�� ���
//					int progress = myPlayer->GetQuestProgress(questID);
//					if (progress == targetNums)
//					{
//						// ����Ʈ �Ϸ�
//						myPlayer->SetQuestState(questID, Protocol::QUEST_STATE_COMPLETED);
//
//						if (_announce == false)
//						{
//							wstring questName = GetQuestScriptInfo(questID).questName;
//							GET_SINGLE(ChatManager)->AddMessage(questName + L" ����Ʈ �Ϸ�.");
//							_announce = true;
//						}
//					}
//					else
//					{
//						// ����Ʈ ���� ��, Ʈ��ŷ
//
//					}
//					break;
//				}
//				}
//			}
//
//		}
//	}
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
