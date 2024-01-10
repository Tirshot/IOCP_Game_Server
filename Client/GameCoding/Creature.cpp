#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "Monster.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"

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
		}
	}
}

void Creature::KnockBack()
{
	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
