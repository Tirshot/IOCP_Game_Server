#pragma once
#include "UI.h"
class Sprite;
class MyPlayer;
class Potion :  public UI
{
	using Super = UI;

public:
	Potion();
	virtual ~Potion();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

protected:
	shared_ptr<MyPlayer> _myPlayer = nullptr;
	shared_ptr<Sprite> _potion = nullptr;
	int _potionCount = 0;
	RECT _rect = {};
};

