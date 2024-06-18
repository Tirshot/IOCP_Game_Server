#pragma once
#include "Panel.h"
class Sprite;
class QuestTrackerUI :	 public Panel
{
	using Super = Panel;

	struct QUEST
	{
		int questID = -1;
		wstring questName = L"";
		wstring description = L"";
		int target = 0;
		int progress = 0;
		bool complete = false;
		bool changed = false;
		shared_ptr<class TextBox> textBox;


		void SetProgress(int progressing) { progress = progressing; }
	};

public:
	QuestTrackerUI();
	virtual ~QuestTrackerUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void AddQuestToTracking(int questID, wstring name, wstring description, int target);
	bool IsQuestInTracker(int questID);

	void SetProgress(int questID, int progress);

public:
	void AddQuestToTracker(shared_ptr<QUEST> quest);
	void RemoveQuestFromTracker(int questID);

public:
	void GetActiveQuests();

private:
	shared_ptr<Sprite> _background = nullptr;
	int _alpha = 128;
	RECT _calcRect = {};

private:
	vector<shared_ptr<QUEST>> _quests;
};

