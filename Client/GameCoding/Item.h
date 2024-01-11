#pragma once
#include "GameObject.h"
class Sprite;
class Item : public GameObject
{
	using Super = GameObject;
public:
	Item();
	virtual ~Item() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle();
	Sprite* _sprite = nullptr;
};

