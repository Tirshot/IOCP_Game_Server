#include "pch.h"
#include "Moblin.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "Player.h"
#include "ClientPacketHandler.h"

Moblin::Moblin()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinMoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinMoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinMoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinMoveRight");

	_flipbookAttack[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinAttackUp");
	_flipbookAttack[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinAttackDown");
	_flipbookAttack[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinAttackLeft");
	_flipbookAttack[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinAttackRight");

	_flipbookHit[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinHitUp");
	_flipbookHit[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinHitDown");
	_flipbookHit[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinHitLeft");
	_flipbookHit[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinHitRight");
}

Moblin::~Moblin()
{
}

void Moblin::BeginPlay()
{
	Super::BeginPlay();
}

void Moblin::Tick()
{
	Super::Tick();
}

void Moblin::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Moblin::TickIdle()
{
	// 길찾기는 서버에서 수행
}

void Moblin::TickMove()
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

void Moblin::TickSkill()
{
	Super::TickSkill();

	// 서버와 동일하게 1초 대기
	auto deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_waitSeconds += deltaTime;

	auto animDuration = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoblinAttackUp")->GetInfo().duration;

	if (_waitSeconds <= animDuration)
		return;

	_waitSeconds = 0.f;

	SetState(IDLE);
}

void Moblin::TickHit()
{
	Super::TickHit();
}

void Moblin::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case SKILL:
		SetFlipbook(_flipbookAttack[info.dir()]);
		break;

	case HIT:
		SetFlipbook(_flipbookHit[info.dir()]);
		break;
	}
}
