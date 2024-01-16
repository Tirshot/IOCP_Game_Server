#pragma once
#include "UI.h"
class Sprite;
class MerchantTutorial : public UI
{
public:
	MerchantTutorial();
	virtual ~MerchantTutorial();

	virtual void BeginPlay();
	virtual void Tick() {};
	virtual void Render(HDC hdc);

private:
	Sprite* _sprite = nullptr;
};

