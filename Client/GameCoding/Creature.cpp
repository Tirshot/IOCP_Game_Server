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

void Creature::KnockBack()
{
	Vec2Int backPos = GetBehindCellPos();

	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(backPos))
		SetCellPos(backPos, true);
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
 