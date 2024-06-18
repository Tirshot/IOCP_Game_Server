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

void Creature::KnockBack(shared_ptr<Creature> attacker)
{
	// 플레이어가 몬스터를 바라보는 방향
	auto dir = attacker->GetLookAtDir(this->GetCellPos());
	Vec2Int backPos = {};

	switch (dir)
	{
	case DIR_UP:
		backPos = GetCellPos() + Vec2Int {0, -1};
		break;

	case DIR_DOWN:
		backPos = GetCellPos() + Vec2Int{ 0, 1 };
		break;

	case DIR_LEFT:
		backPos = GetCellPos() + Vec2Int{ -1, 0 };
		break;

	case DIR_RIGHT:
		backPos = GetCellPos() + Vec2Int{ 1, 0 };
		break;

	default:
		return;
	}

	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(backPos))
		SetCellPos(backPos, true);
}
