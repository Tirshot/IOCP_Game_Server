#pragma once
#include "GameObject.h"

class DeathEffect : public GameObject
{
	using Super = GameObject;

public:
	DeathEffect();
	virtual ~DeathEffect() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	virtual void UpdateAnimation()  override;
};
