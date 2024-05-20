#pragma once
#include "Panel.h"
class Sprite;
class QuestTrackerUI :	 public Panel
{
	using Super = Panel;

public:
	QuestTrackerUI();
	virtual ~QuestTrackerUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

private:
	wstring _questName = L"";
	int _progress = 0;
	int _target = 0;
	Sprite* _backGround = nullptr;
};

