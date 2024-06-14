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
	// �÷��̾ ���͸� �ٶ󺸴� ����
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

	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(backPos))
		SetCellPos(backPos, true);
}
