#pragma once
#include "Projectile.h"

class Arrow : public Projectile, public enable_shared_from_this<Arrow>
{
	using Super = Projectile;
public:
	Arrow();
	virtual ~Arrow() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	virtual void TickIdle() override;
	virtual void TickMove()  override;
	virtual void TickHit()  override;
	virtual void UpdateAnimation()  override;

private:
	shared_ptr<Flipbook> _flipbookMove[4] = {};
};

