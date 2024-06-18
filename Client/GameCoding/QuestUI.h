#pragma once
#include "Panel.h"
class Texture;
class QuestUI : public Panel
{
	using Super = Panel;

public:
	QuestUI();
	virtual ~QuestUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void OnClickBackButton();
	void OnClickExitButton();
	void OnClickCountMinusButton();
	void OnClickCountPlusButton();

	void ResetQuestList();
	void ResetPage() { _page = 1; }
	void RepostionPanels();

private:
	void SetQuestPanel(pair<int, Protocol::QuestInfo> questInfo);

protected:
	RECT _rect = {};
	shared_ptr<Texture> _background = nullptr;
	map<int, Protocol::QuestInfo> _quests;
	int _idx = 1;
	int _page = 1;
	int _maxPage = 1;
};

