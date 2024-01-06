#include "Monster.h"
#include "pch.h"
#include "Monster.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "Player.h"
#include "HitEffect.h"
#include "ClientPacketHandler.h"

Monster::Monster()
{
	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRight");

	_flipbookHit[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUpHit");
	_flipbookHit[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDownHit");
	_flipbookHit[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeftHit");
	_flipbookHit[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRightHit");
	// type도 protobuf로 받아옴
	// _type = CreatureType::Monster;
	
	// 스탯도 protobuf로 받아옴
	//info.set_hp(50);
	//info.set_maxhp(50);
	//info.set_attack(10);
	//info.set_defence(0);
}

Monster::~Monster()
{

}
void Monster::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void Monster::Tick()
{
	Super::Tick();

}

void Monster::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Monster::TickIdle()
{
	// 길찾기는 서버에서 수행
}

void Monster::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 5.f)
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		// 더 먼 쪽을 바라봄
		bool horizontal = abs(dir.x) > abs(dir.y);
		if (horizontal)
			SetDir(dir.x < 0 ? DIR_LEFT : DIR_RIGHT);
		else
			SetDir(dir.y < 0 ? DIR_UP : DIR_DOWN);

		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 100 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 100 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 100 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 100 * deltaTime;
			break;
		}
	}
}

void Monster::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (_waitSeconds > 0)
	{
		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_waitSeconds = max(0, _waitSeconds - deltaTime);
		return;
	}

	// 공격 판정
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return;

	auto* creature = scene->GetCreatureAt(GetFrontCellPos());
	if (creature)
		creature->OnDamaged(this);

	SetState(IDLE);
}

void Monster::TickHit()
{
 	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	_now = GetTickCount64();

	if (_now > _wait)
		SetState(IDLE);
}

void Monster::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case HIT:
		SetFlipbook(_flipbookHit[info.dir()]);
		break;
	}
}

void Monster::KnockBack()
{
	auto dir = GetCellPos() - GetFrontCellPos();

	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(GetCellPos() + dir))
		SetCellPos(GetCellPos() + dir, true);
}