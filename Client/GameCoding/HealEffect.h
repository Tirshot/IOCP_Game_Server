#pragma once
#include "GameObject.h"
class HealEffect :  public GameObject, public enable_shared_from_this<HealEffect>
{
	using Super = GameObject;

public:
	HealEffect();
	virtual ~HealEffect() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	virtual void UpdateAnimation()  override;
};

