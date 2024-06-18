#pragma once
#include "UI.h"
class Sprite;
class MyPlayer;
class HP : public UI
{
	using Super = UI;

public:
	HP();
	virtual ~HP();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

protected:
	Vec2Int _size = { 52, 52 };
	shared_ptr<MyPlayer> _myPlayer = nullptr;
	int _hp = 0;
	int _maxHp = 0;
	shared_ptr<Sprite> _blackHeart = nullptr;
	shared_ptr<Sprite> _heart = nullptr;
};

