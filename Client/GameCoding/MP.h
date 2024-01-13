#pragma once
#include "UI.h"
class Texture;
class MyPlayer;
class MP : public UI
{
	using Super = UI;

public:
	MP();
	virtual ~MP();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

protected:
	Vec2Int _size = { 52, 52 };
	MyPlayer* _myPlayer = nullptr;
	int _mp = 0;
	int _maxMp = 0;
	Texture* _blackMp = nullptr;
	Texture* _blueMp = nullptr;
	float _sumTime = 0.f;
	float _deltaTime = 0.f;
};

