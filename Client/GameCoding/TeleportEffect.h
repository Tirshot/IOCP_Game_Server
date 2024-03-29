#pragma once
#include "GameObject.h"

class TeleportEffect : public GameObject
{
	using Super = GameObject;

public:
	TeleportEffect();
	virtual ~TeleportEffect() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	virtual void UpdateAnimation()  override;
};

