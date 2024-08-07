#include "pch.h"
#include "Monster.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "DevScene.h"
#include "Tilemap.h"
#include "Player.h"
#include "HitEffect.h"
#include "ClientPacketHandler.h"

Monster::Monster()
{
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
	}
}

void Monster::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (_isEffectSpawned)
		return;

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		auto pos = GetFrontCellPos();
		auto creature = scene->GetCreatureAt(pos);
		if (creature)
		{
			scene->SpawnObject<HitEffect>(pos);
			_isEffectSpawned = true;
		}
	}
}

void Monster::TickHit()
{
	_now = GetTickCount64();

	if (_now > _wait)
		SetState(IDLE);
}