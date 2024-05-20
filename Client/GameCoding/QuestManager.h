#pragma once

struct QuestInfo
{
	int questID;
	wstring questName;
	wstring questNPC;
	wstring description;
	wstring questScript;
};

class QuestManager
{
	DECLARE_SINGLE(QuestManager);

public:
	~QuestManager();

	virtual void BeginPlay();
	virtual void Tick();

public:
	QuestInfo GetQuestScriptInfo(int questID);
	vector<wstring> GetQuestScript(int questID);

private:
	// 퀘스트 트래커
	// 수락한 퀘스트
	map<int, Protocol::QuestInfo> _quests;
	bool _announce = false;
};

