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
	virtual void UpdateHit();
	virtual void UpdateSpin();
	virtual void UpdateTeleport();

public:
	virtual void OnDamaged(CreatureRef attacker, bool debug = false) override;

public:
	void MakeArrow();
	void Teleport();
	void QuestProgress(int questid);
	void KillQuestProgress(Protocol::MONSTER_TYPE monsterType);
	void ItemQuestProgress(int itemID);

public:
	map<int, class PlayerQuestState> GetAcceptedQuests();
	Protocol::QUEST_STATE GetQuestState(int questId);

public:
	GameSessionRef session;

private:
	// 초당 마나 회복 계산
	uint64 _now = 0;
	uint64 _prev = 0;
};

