#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class Button;
class QuestUIPanel : public Panel
{
	using Super = Panel;

public:

	QuestUIPanel();
	QuestUIPanel(Protocol::QuestInfo& info, Vec2 pos, int idx);

	virtual ~QuestUIPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

public:
	int GetQuestId() { return _questId; }
	void SetQuestId(int id) { _questId = id; }

	int GetIndex() { return _index; }
	RECT GetRect() { return _rect; }

	int GetRewardGold() { return _reward; }
	int GetRewardItem() { return _rewardItem; }
	int GetRewardItemNum() { return _rewardItemNum; }

	Protocol::QUEST_STATE GetQuestState() { if (this != nullptr) return _questState; }

protected:
	shared_ptr<Texture> _background = nullptr;
	shared_ptr<Sprite> _questImage = nullptr;
	shared_ptr<Texture> _goldImage = nullptr;
	shared_ptr<Button> _accept = nullptr;
	shared_ptr<Button> _complete = nullptr;

protected:
	RECT _rect = {};
	int _questId = 0;
	int _targetId = 0;
	wstring _questName = L"";
	wstring _questNPC = L"";
	wstring _description = L"";
	vector<wstring> _scripts = {};
	Protocol::OBJECT_TYPE _objectType = Protocol::OBJECT_TYPE_NONE;
	int _targetNums = 0;
	Protocol::QUEST_STATE _questState = Protocol::QUEST_STATE_IDLE;
	int _reward = 0;
	int _rewardItem = 0;
	int _rewardItemNum = 0;
	int _index = 0;
	bool _isLinkQuest = false;
	int _prevQuestID = -1;
};

