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
	void OnClickRefreshButton();

	void ResetQuestList();

protected:
	RECT _rect = {};
	Texture* _background = nullptr;
	map<int, Protocol::QuestInfo> _quests;
	int _idx = 1;
};

