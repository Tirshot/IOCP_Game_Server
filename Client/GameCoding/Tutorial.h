#pragma once
#include "UI.h"
class Sprite;
class Tutorial : public UI
{
public:
	Tutorial();
	virtual ~Tutorial();

	virtual void BeginPlay();
	virtual void Tick() {};
	virtual void Render(HDC hdc);

private:
	Sprite* _sprite = nullptr;
};

