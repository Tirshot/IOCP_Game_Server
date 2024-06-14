#pragma once
#include "Monster.h"
class Moblin : public Monster
{
	using Super = Monster;

public:
	Moblin();
	virtual ~Moblin() override;

	virtual void Init();
	virtual void Update();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateHit();
};

