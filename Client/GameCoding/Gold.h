#pragma once
#include "UI.h"
class Sprite;
class MyPlayer;
class Gold : public UI
{
	using Super = UI;

public:
	Gold();
	virtual ~Gold();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

protected:
	Vec2Int _size = { 23, 24 };
	shared_ptr<MyPlayer> _myPlayer = nullptr;
	shared_ptr<Sprite> _golds = nullptr;
	int _gold = 0;
	RECT _rect = {};
};

