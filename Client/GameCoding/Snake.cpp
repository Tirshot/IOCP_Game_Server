#include "pch.h"
#include "Snake.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "Player.h"
#include "ClientPacketHandler.h"

Snake::Snake()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeIdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeIdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeIdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeIdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRight");

	_flipbookHit[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUpHit");
	_flipbookHit[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDownHit");
	_flipbookHit[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeftHit");
	_flipbookHit[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRightHit");
}

Snake::~Snake()
{
}

void Snake::BeginPlay()
{
	Super::BeginPlay();
}

void Snake::Tick()
{
	Super::Tick();
}

void Snake::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Snake::TickIdle()
{
	// 길찾기는 서버에서 수행
}

void Snake::TickMove()
{
	Super::TickMove();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	float speed = info.speed();

	switch (info.dir())
	{
	case DIR_UP:
		_pos.y -= speed * deltaTime;
		break;
	case DIR_DOWN:
		_pos.y += speed * deltaTime;
		break;
	case DIR_LEFT:
		_pos.x -= speed * deltaTime;
		break;
	case DIR_RIGHT:
		_pos.x += speed * deltaTime;
		break;
	}
}

void Snake::TickSkill()
{
	Super::TickSkill();

	// 서버와 동일하게 1초 대기
	auto deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_waitSeconds += deltaTime;

	auto animDuration = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp")->GetInfo().duration;

	if (_waitSeconds <= animDuration)
		return;

	_waitSeconds = 0.f;

	SetState(IDLE);
}

void Snake::TickHit()
{
	Super::TickHit();
}

void Snake::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case HIT:
		SetFlipbook(_flipbookHit[info.dir()]);
		break;
	}
}
