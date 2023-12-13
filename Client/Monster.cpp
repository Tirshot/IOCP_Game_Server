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

Monster::Monster()
{
	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRight");

	_stat.hp = 50;
	_stat.maxHp = 50;
	_stat.attack = 10;
	_stat.defence = 0;
}

Monster::~Monster()
{
}

void Monster::BeginPlay()
{
	Super::BeginPlay();

	SetState(ObjectState::Move);
	SetState(ObjectState::Idle);
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
	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
	if (scene == nullptr)
		return;

	// 주변에 있는 플레이어 찾기
	if (_target == nullptr)
		_target = scene->FindClosestPlayer(GetCellPos());

	if (_target)
	{
		// 공격 판단
		Vec2Int dir = _target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			// 공격하기
			SetDir(GetLookAtDir(_target->GetCellPos()));
			SetState(ObjectState::Skill);
			_waitSeconds = 0.5f;
		}
		else
		{
			// 좌표 찾기
			vector<Vec2Int> path;
			if (scene->FindPath(GetCellPos(), _target->GetCellPos(), OUT path))
			{
				if (path.size() > 1)
				{
					// 한 칸만 이동
					Vec2Int nextPos = path[1];
					if (scene->CanGo(nextPos))
					{
						SetCellPos(nextPos);
						SetState(ObjectState::Move);
					}
				}
				else
					// 길을 못 찾았으면 정지
					SetCellPos(path[0]);
			}
		}
	}
}

void Monster::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 3.f)
	{
		SetState(ObjectState::Idle);
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

		switch (_dir)
		{
		case DIR_UP:
			_pos.y -= 50 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 50 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 50 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 50 * deltaTime;
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
	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
	if (scene == nullptr)
		return;

	auto* creature = scene->GetCreatureAt(GetFrontCellPos());

	if (creature)
	{
		scene->SpawnObject<HitEffect>(GetFrontCellPos());
		// 데미지 피격
		creature->OnDamaged(this);
	}

	SetState(ObjectState::Idle);
}

void Monster::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[_dir]);
}
