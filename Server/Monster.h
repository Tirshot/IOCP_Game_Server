#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
	using Super = GameObject;

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
	virtual void KnockBack();

protected:
	uint64 _waitUntil = 0;
	weak_ptr<Player> _target;
};

