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
	// type�� protobuf�� �޾ƿ�
	// _type = CreatureType::Monster;
	
	// ���ȵ� protobuf�� �޾ƿ�
	//info.set_hp(50);
	//info.set_maxhp(50);
	//info.set_attack(10);
	//info.set_defence(0);
}

Monster::~Monster()
{
	GET_SINGLE(SoundManager)->Play(L"MonsterOnDamaged");
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
	// ��ã��� �������� ����
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
		// �� �� ���� �ٶ�
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

	if (_waitSeconds > 0)
	{
		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_waitSeconds = max(0, _waitSeconds - deltaTime);
		return;
	}

	// ���� ����
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return;

	auto player = dynamic_pointer_cast<Player>(scene->GetCreatureAt(GetFrontCellPos()));
	
	// Player�� �ǰ� ����Ʈ ���
	scene->SpawnObject<HitEffect>(GetFrontCellPos());
	GET_SINGLE(SoundManager)->Play(L"PlayerOnDamaged");
	SetState(IDLE);
}

void Monster::TickHit()
{
	_now = GetTickCount64();

	if (_now > _wait)
		SetState(IDLE);
}