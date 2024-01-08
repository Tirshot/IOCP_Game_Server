#pragma once
#include "UI.h"
class GameOver : public UI
{
public:
	GameOver();
	virtual ~GameOver();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

protected:
	Vec2	_pos = { 0, 0 };
	Vec2Int _size = { 800, 600 };
};

