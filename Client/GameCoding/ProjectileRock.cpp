#include "pch.h"
#include "ProjectileRock.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Creature.h"
#include "Monster.h"
#include "HitEffect.h"
#include "ChatManager.h"
#include "SoundManager.h"

ProjectileRock::ProjectileRock()
{
	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ProjectileRockUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ProjectileRockDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ProjectileRockLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ProjectileRockRight");
}

ProjectileRock::~ProjectileRock()
{

}

void ProjectileRock::BeginPlay()
{
	Super::BeginPlay();
}

void ProjectileRock::Tick()
{
	Super::Tick();
}

void ProjectileRock::Render(HDC hdc)
{
	Super::Render(hdc);
}

void ProjectileRock::TickIdle()
{
	auto scene = dynamic_pointer_cast<DevScene>(GET_SINGLE(SceneManager)->GetCurrentScene());
	if (scene == nullptr)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	// 다음 위치에 갈 수 있는지 확인
	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
		return;
	}
	else
	{
		// 앞이 비어있으면 전진
		auto creature = scene->GetCreatureAt(nextPos);
		if (creature == _owner)
		{
			SetCellPos(nextPos);
			SetState(MOVE);
			return;
		}
	}

	// 앞이 비어있지 않으며 발사체 주인이 아닐 경우 타격
	SetState(HIT);
}

void ProjectileRock::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 3.f)
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 800 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 800 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 800 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 800 * deltaTime;
			break;
		}
	}
}

void ProjectileRock::TickHit()
{
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int pos = Vec2Int{ info.posx(), info.posy() };
	Vec2Int nextPos = pos + deltaXY[info.dir()];

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene == nullptr)
		return;

	// 앞이 비어있으면 전진, 몬스터라면 타격
	/*Monster* _target = dynamic_cast<Monster*>(scene->GetCreatureAt(nextPos));*/
	auto creature = scene->GetCreatureAt(GetFrontCellPos());

	if (creature)
		scene->SpawnObject<HitEffect>(GetFrontCellPos());

	scene->RemoveActor(shared_from_this());
}

void ProjectileRock::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[info.dir()]);
}