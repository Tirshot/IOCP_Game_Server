#pragma once
class Quest
{
public:
	Quest();
	virtual ~Quest();

	virtual void Init();
	virtual void Accept();
	virtual void Completed();

	static QuestRef CreateQuest();

public:
	Protocol::QuestInfo info;
};
extern int g_questIdGenerator;