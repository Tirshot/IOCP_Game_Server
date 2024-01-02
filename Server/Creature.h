#pragma once
#include "GameObject.h"

class Creature : public GameObject, public enable_shared_from_this<Creature>
{
	using Super = GameObject;

public:
	Creature();
	virtual ~Creature();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	void OnDamaged(CreatureRef attacker);
	void KnockBack();
	uint64 SetWait(uint64 time) { return _waitHit = GetTickCount64() + time; }

protected:
	uint64 _waitUntil = 0;
	uint64 _waitHit = 0;
};

