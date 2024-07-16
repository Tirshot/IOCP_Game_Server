#pragma once
#include "Projectile.h"
class Arrow : public Projectile, public enable_shared_from_this<Arrow>
{
	using Super = Projectile;
public:
	Arrow();
	virtual ~Arrow() override;

	void BeginPlay();
	void Tick();

public:
	void TickIdle();
	void TickMove();
	void TickHit();

public:
	MonsterRef& GetTarget() { return _target; }

protected:
	float _waitUntil = 0;
	MonsterRef _target;
};

