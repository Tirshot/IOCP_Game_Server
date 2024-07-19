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

	virtual void OnDamaged(CreatureRef attacker, bool debug = false);
	virtual void KnockBack(Protocol::DIR_TYPE dir);
	uint64 SetWait(uint64 time) { return _waitHit = GetTickCount64() + time; }
	wstring GetName();

protected:
	uint64 _waitUntil = 0;
	uint64 _waitHit = 0;
};

