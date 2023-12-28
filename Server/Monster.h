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

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateHit();

public:
	uint64 SetWait(uint64 time) { return _waitHit = GetTickCount64() + time; }

protected:
	uint64 _waitUntil = 0;
	uint64 _waitHit = 0;
	weak_ptr<Player> _target;
};

