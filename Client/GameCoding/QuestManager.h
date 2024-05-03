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
	QuestInfo GetQuestInfo(int questID);
	vector<wstring> GetQuestScript(int questID);
private:
	// 퀘스트 트래커

};

