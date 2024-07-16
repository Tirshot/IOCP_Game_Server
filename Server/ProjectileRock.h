#pragma once
#include "Projectile.h"
class ProjectileRock :  public Projectile, public enable_shared_from_this<ProjectileRock>
{
	using Super = Projectile;
public:
	ProjectileRock();
	virtual ~ProjectileRock() override;

	void BeginPlay();
	void Tick();

public:
	void TickIdle();
	void TickMove();
	void TickHit();

public:
	PlayerRef& GetTarget() { return _target; }

protected:
	float _waitUntil = 0;
	PlayerRef _target;
};

