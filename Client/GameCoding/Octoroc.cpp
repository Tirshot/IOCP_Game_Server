#include "pch.h"
#include "Octoroc.h"
#include "ProjectileRock.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Player.h"
#include "ClientPacketHandler.h"

Octoroc::Octoroc()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocIdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocIdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocIdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocIdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocMoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocMoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocMoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocMoveRight");

	_flipbookSkill[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocSkillUp");
	_flipbookSkill[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocSkillDown");
	_flipbookSkill[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocSkillLeft");
	_flipbookSkill[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocSkillRight");
}

Octoroc::~Octoroc()
{
	_flipbookIdle->reset();
	_flipbookMove->reset();
	_flipbookSkill->reset();
}

void Octoroc::BeginPlay()
{
	Super::BeginPlay();
}

void Octoroc::Tick()
{
	Super::Tick();
}

void Octoroc::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Octoroc::TickIdle()
{
	// 길찾기는 서버에서 수행
}

void Octoroc::TickMove()
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

void Octoroc::TickSkill()
{
	Super::TickSkill();

	// 서버와 동일하게 1초 대기
	auto deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_waitSeconds += deltaTime;

	auto animDuration = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_OctorocAttackUp")->GetInfo().duration;

	if (_waitSeconds <= animDuration)
		return;

	_waitSeconds = 0.f;
	_isEffectSpawned = false;

	SetState(IDLE);
}

void Octoroc::TickHit()
{
	Super::TickHit();
}

void Octoroc::UpdateAnimation()
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
		SetFlipbook(_flipbookMove[info.dir()]);
		break;
	}
}

void Octoroc::MakeRock()
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		auto rock = scene->SpawnObject<ProjectileRock>({ info.posx(), info.posy() });
		rock->info.set_dir(info.dir());
		rock->SetOwner(shared_from_this());
	}
}
