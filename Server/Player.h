#pragma once
#include "Creature.h"
class Player : public Creature
{
	using Super = Creature;

public:
	Player();
	virtual ~Player();

	virtual void Init();
	virtual void Update();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateSpin();
	virtual void UpdateTeleport();

public:
	void MakeArrow();
	void Teleport();

public:
	void QuestProgress(int questid);
	pair<Protocol::QUEST_STATE, int > GetQuestState(int questid) { return _questsStates[questid]; }
	void SetQuestState(int questid, Protocol::QUEST_STATE state, int progress) { _questsStates[questid] = { state, progress }; }
	
	int GetQuestProgress(int questId);
	void SetQuestProgress(int questId, int progress);
	void AddQuestProgress(int questId) { _questsStates[questId].second += 1; }

public:
	GameSessionRef session;

private:
	// 초당 마나 회복 계산
	uint64 _now = 0;
	uint64 _prev = 0;
	
	map<int, pair<Protocol::QUEST_STATE, int>> _questsStates;
};

