#pragma once
#include "Creature.h"

class Monster : public Creature
{
	using Super = Creature;

public:
	Monster();
	virtual ~Monster() override;

	virtual void Init();
	virtual void Update();

protected:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateHit();

	void ItemDrop(CreatureRef owner);
	void GoldDrop(CreatureRef owner);

public:
	virtual void OnDamaged(CreatureRef attacker, bool debug = false) override;
	uint64 SetWait(uint64 time) { return _waitHit = GetTickCount64() + time; }
	wstring GetName();
	virtual void KnockBack() override;
	void MonsterQuestProgress(PlayerRef player);

protected:
	uint64 _waitUntil = 0;
	uint64 _waitHit = 0;
	weak_ptr<Player> _target;
	Vec2Int _initialPos = {};
};

