#include "pch.h"
#include "Arrow.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Creature.h"
#include "Monster.h"
#include "Player.h"
#include "HitEffect.h"
#include "ChatManager.h"

Arrow::Arrow()
{
	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowRight");
}

Arrow::~Arrow()
{

}

void Arrow::BeginPlay()
{
	Super::BeginPlay();
}

void Arrow::Tick()
{
	Super::Tick();
}

void Arrow::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Arrow::TickIdle()
{
	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
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
		// 앞이 비어있으면 전진, 몬스터라면 타격
		Creature* creature = scene->GetCreatureAt(nextPos);
		_owner->GetFrontCellPos();
		if (creature == _owner)
		{
			SetCellPos(nextPos);
			SetState(MOVE);
			return;
		}
		 // 이펙트 출력
		if (creature && creature->GetType() == Protocol::OBJECT_TYPE_MONSTER)
		{
			scene->SpawnObject<HitEffect>(nextPos);
			creature->OnDamaged(_owner);
			//creature->SetWait(0);
			//creature->SetState(HIT);
		}
	}
	scene->RemoveActor(this);
}

void Arrow::TickMove()
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

void Arrow::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[info.dir()]);
}
