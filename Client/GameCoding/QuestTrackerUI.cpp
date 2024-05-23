#include "pch.h"
#include "TextBox.h"
#include "QuestTrackerUI.h"
#include "QuestManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

QuestTrackerUI::QuestTrackerUI()
{
}

QuestTrackerUI::~QuestTrackerUI()
{
}

void QuestTrackerUI::BeginPlay()
{
	_size = { 150, 150 };
	_pos.x = 800 - (int)_size.x;
	_pos.y = 120;

	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");

	_rect = {};
	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _rect.left + _size.x - 10;
	_rect.bottom = _rect.top + _size.y;
}

void QuestTrackerUI::Tick()
{
	for (size_t i = 0; i < _quests.size(); ++i)
	{
		auto& box = _quests[i]->textBox;
		if (i == 0)
		{
			// ù ��° TextBox�� _pos ��ġ�� ��ġ
			box->SetPos({ _pos.x, _pos.y });
		}
		else
		{
			// ���� TextBox�� ��ġ�� ũ�⸦ ������
			auto& prevBox = _quests[i - 1]->textBox;
			auto prevRect = prevBox->GetRect();

			// ���� TextBox�� ���� TextBox�� �Ʒ��� ��ġ
			box->SetPos({ _pos.x, (float)prevRect.bottom });
		}
	}
}

void QuestTrackerUI::Render(HDC hdc)
{
	if (_visible)
	{
		for (auto& child : _children)
			child->Render(hdc);
	}
}

void QuestTrackerUI::AddQuestToTracking(int questID, wstring name, wstring description, int target)
{
	QUEST* quest = new QUEST();
	quest->questName = name;
	quest->description = description;
	quest->target = target;
	quest->questID = questID;

	auto& progress = quest->progress;
	auto& complete = quest->complete;
	wstring completeStr = L"";

	wstring str = std::format(L"{0} {4}\n{1}\n({2} / {3})\n", name, description, progress, target, completeStr);

	TextBox* list = new TextBox();
	list->SetParent(this);
	list->SetBackground(_background);
	list->SetPos({ _pos.x, _pos.y });
	list->SetPadding(5, 5);
	list->SetTransparent(true);
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

void QuestTrackerUI::AddQuestToTracker(QUEST* quest)
{
	_quests.push_back(quest);
}

void QuestTrackerUI::RemoveQuestFromTracker(int questID)
{
	auto it = std::remove_if(_quests.begin(), _quests.end(),
		[&](QUEST* quest)
		{
			if (quest->questID == questID)
			{
				RemoveChild(quest->textBox);
				delete quest->textBox; // TextBox �޸� ����
				delete quest; // QUEST �޸� ����
				return true;
			}
			return false;
		});

	if (it != _quests.end())
	{
		_quests.erase(it, _quests.end());
	}

}
