#include "pch.h"
#include "TextBox.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "QuestTrackerUI.h"
#include "QuestManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "Sprite.h"

QuestTrackerUI::QuestTrackerUI()
{
}

QuestTrackerUI::~QuestTrackerUI()
{
	_quests.clear();
	_background.reset();
}

void QuestTrackerUI::BeginPlay()
{
	Super::BeginPlay();

	_size = { 150, 150 };
	_pos.x = 800 - (int)_size.x;
	_pos.y = 150;

	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");

	_rect = {};
	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _rect.left + _size.x - 10;
	_rect.bottom = _rect.top + _size.y;
}
float _sumTime = 0.f;
void QuestTrackerUI::Tick()
{
	Super::Tick();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_sumTime += deltaTime;

	if (_sumTime <= 0.1f)
		return;

	for (size_t i = 0; i < _quests.size(); ++i)
	{
		auto& box = _quests[i]->textBox;
		if (i == 0)
		{
			// 첫 번째 TextBox는 _pos 위치에 배치
			box->SetPos({ _pos.x, _pos.y });
		}
		else
		{
			// 이전 TextBox의 위치와 크기를 가져옴
			auto& prevBox = _quests[i - 1]->textBox;
			auto prevRect = prevBox->GetRect();

			// 현재 TextBox를 이전 TextBox의 아래에 배치
			box->SetPos({ _pos.x, (float)prevRect.bottom });
		}

		if (_quests[i]->target <= 0)
		{
			int questID = _quests[i]->questID;

			auto scene = GET_SINGLE(SceneManager)->GetDevScene();
			if (scene)
			{
				auto& quest = scene->GetQuest(questID);
				auto target = quest.targetnums();

				if (target <= 0)
					continue;

				_quests[i]->target = target;
			}
		}
	}
	_sumTime = 0.f;
}

void QuestTrackerUI::Render(HDC hdc)
{
	Super::Render(hdc);
}

void QuestTrackerUI::AddQuestToTracking(int questID, wstring name, wstring description, int target)
{
	auto quest = make_shared<QUEST>();
	quest->questName = name;
	quest->description = description;
	quest->target = target;
	quest->questID = questID;

	auto& progress = quest->progress;
	auto& complete = quest->complete;
	wstring completeStr = L"";

	wstring str = std::format(L"{0} {4}\n{1}\n({2} / {3})\n", name, description, progress, target, completeStr);

	auto list = make_shared<TextBox>();
	list->SetParent(shared_from_this());
	list->SetBackground(_background);
	list->SetPos({ _pos.x, _pos.y });
	list->SetPadding(5, 5);
	list->SetTransparent(false);
	list->SetAlpha(150);
	list->SetVisible(true);
	list->SetText(str);
	list->SetSize({ _size.x, 20 * 3 });
	list->SetPos({ (float)_pos.x, _pos.y + ((float)_quests.size() * (float)list->GetSize().y) });

	AddChild(list);
	quest->textBox = list;
	_quests.push_back(quest);
}

bool QuestTrackerUI::IsQuestInTracker(int questID)
{
	if (_quests.empty())
		return false;

	for (const auto& quest : _quests)
	{
		if (quest->questID == questID)
			return true;
	}
	return false;
}


void QuestTrackerUI::SetProgress(int questID, int progress)
{
	for (auto& quest : _quests)
	{
		if (quest->questID == questID)
		{
			quest->progress = progress;

			wstring str = std::format(L"{0} \n{1}\n({2} / {3})\n", quest->questName, quest->description, progress, quest->target);
			quest->textBox->SetText(str);
		}
	}
}

void QuestTrackerUI::AddQuestToTracker(shared_ptr<QUEST> quest)
{
	_quests.push_back(quest);
}

void QuestTrackerUI::RemoveQuestFromTracker(int questID)
{
	auto it = std::remove_if(_quests.begin(), _quests.end(),
		[&](shared_ptr<QUEST> quest)
		{
			if (quest->questID == questID)
			{
				RemoveChild(quest->textBox);
				return true;
			}
			return false;
		});

	if (it != _quests.end())
	{
		_quests.erase(it, _quests.end());
	}

}

void QuestTrackerUI::GetActiveQuests()
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (scene == nullptr || myPlayer == nullptr)
		return;

	auto activeQuest = myPlayer->GetActiveQuests();

	if (activeQuest.empty())
		return;

	for (auto& quest : activeQuest)
	{
		int questID = quest.first;
		auto state = quest.second.first;
		int progress = quest.second.second;
	}
}
