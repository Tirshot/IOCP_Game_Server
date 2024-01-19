#include "pch.h"
#include "Quest.h"

int g_questIdGenerator = 0;
Quest::Quest()
{
}

Quest::~Quest()
{
}

void Quest::Init()
{
}

void Quest::Accept()
{
}

void Quest::Completed()
{
}

QuestRef Quest::CreateQuest()
{
	QuestRef quest = make_shared<Quest>();
	quest->info.set_questid(g_questIdGenerator++);
	quest->info.set_queststate(Protocol::QUEST_STATE_IDLE);

	return quest;
}
