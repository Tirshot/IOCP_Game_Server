#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class Button;
class QuestUIPanel : public Panel
{
	using Super = UI;

public:
	QuestUIPanel();
	QuestUIPanel(Protocol::QuestInfo& info, Vec2 pos, int idx);

	virtual ~QuestUIPanel();
	
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void OnClickAcceptButton();
	void OnClickCompleteButton();

public:
	int GetQuestId() { return _questId; }
	void SetQuestId(int id) { _questId = id; }

	int GetIndex() { return _index; }
	RECT GetRect() { return _rect; }

protected:
	Texture* _background = nullptr;
	Sprite* _questImage = nullptr;
	Texture* _goldImage = nullptr;
	Button* _accept = nullptr;
	Button* _complete = nullptr;

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
};

