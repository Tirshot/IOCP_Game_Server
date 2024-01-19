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

void Creature::OnDamaged(Creature* attacker)
{
	if (attacker == nullptr)
		return;

	// disable PvP : ������ ���� �Ұ�
	if (GetType() == attacker->GetType())
		return;

	// NPC ���� �Ұ�
	if (info.objecttype() == Protocol::OBJECT_TYPE_NPC || info.objecttype() == Protocol::OBJECT_TYPE_NPC_SIGN)
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	// hp�� �׻� ���
	info.set_hp(max(0, info.hp() - damage));
	if (info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
	{
		GET_SINGLE(SoundManager)->Play(L"PlayerOnDamaged");
	}
	else if (info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
	{
		GET_SINGLE(SoundManager)->Play(L"MonsterOnDamaged");
	}
	{	// �ǰ� ��Ŷ ����
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Hit(info, attacker->info.objectid());
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (info.hp() == 0)
	{
		if (scene)
		{
			scene->RemoveActor(this);
			scene->SpawnObject<DeathEffect>(GetCellPos());
		}
	}
}

void Creature::KnockBack(Creature* attacker)
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
