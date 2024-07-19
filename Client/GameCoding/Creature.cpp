#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "DeathEffect.h"
#include "Monster.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "SoundManager.h"

Creature::Creature()
{
}

Creature::~Creature()
{
}

void Creature::BeginPlay()
{
	Super::BeginPlay();
}

void Creature::Tick()
{
	Super::Tick();
}

void Creature::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Creature::KnockBack(Protocol::DIR_TYPE dir)
{
	Vec2Int cellPos = {};

	// 상대가 바라보는 방향으로 넉백 당함
	switch (dir)
	{
	case DIR_UP:
		cellPos = { 0, -1 };
		break;

	case DIR_DOWN:
		cellPos = { 0, 1 };
		break;

	case DIR_LEFT:
		cellPos = { -1, 0 };
		break;

	case DIR_RIGHT:
		cellPos = { 1, 0 };
		break;

	default:
		cellPos = { 0, 0 };
		break;
	}

	Vec2Int newPos = GetCellPos() + cellPos;

	if (CanGo(newPos))
	{
		SetCellPos(newPos, true);
	}
}

bool Creature::IsSafeZone(Vec2Int cellPos)
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		return scene->IsSafeZone(cellPos);
	}

	return false;
}
 