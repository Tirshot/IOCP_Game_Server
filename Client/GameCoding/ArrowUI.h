#pragma once
#include "UI.h"
class MyPlayer;
class Sprite;
class ArrowUI : public UI
{
	using Super = UI;

public:
	ArrowUI();
	virtual ~ArrowUI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

protected:
	MyPlayer* _myPlayer = nullptr;
	Sprite* _arrows= nullptr;
	int32 _arrowsCount = 0;
	RECT _rect = {};
};

