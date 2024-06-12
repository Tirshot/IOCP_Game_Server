#pragma once
#include "Monster.h"
class Snake :  public Monster
{
	using Super = Monster;

public:
	Snake();
	virtual ~Snake() override;

	virtual void Init();
	virtual void Update();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateHit();
};

