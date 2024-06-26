#pragma once
#include "Monster.h"
class Octoroc : public Monster
{
	using Super = Monster;

public:
	Octoroc();
	virtual ~Octoroc() override;

	virtual void Init();
	virtual void Update();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateHit();

public:
	void MakeRock();
};

