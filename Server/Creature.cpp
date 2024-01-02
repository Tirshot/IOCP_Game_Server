#include "pch.h"
#include "Creature.h"
#include "GameRoom.h"
#include "Chat.h"

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

void Creature::OnDamaged(CreatureRef attacker)
{
	if (attacker == nullptr)
		return;

	if (shared_from_this() == nullptr)
		return;

	// disable PvP : ������ ���� �Ұ�
	if (info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	//// hp�� �׻� ���
 //	info.set_hp(max(0, info.hp() - damage));

	// ä�� ���
	{
		wstring attackerType = L"";
		wstring objectType = L"";

		if (attacker->info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
		{
			attackerType = L"Player";
			objectType = L"Monster";
		}
		else
		{
			attackerType = L"Monster";
			objectType = L"Player";
		}

		GChat->AddText(format(L"{0} {1}��(��) {2} {3}���� {4}�� ���ظ� ����",
			attackerType,
			attacker->GetObjectID(),
			objectType,
			GetObjectID(),
			damage));

		GChat->AddText(attacker->GetObjectID(), format(L"{0} {1}���� {2}�� ���ظ� ����",
			objectType,
			GetObjectID(),
			damage));

		if (info.hp() == 0)
		{
			if (room)
			{
				room->RemoveObject(GetObjectID());
				GChat->AddText(format(L"{0} {1}��(��) {2} {3}�� ���� ������",
					objectType,
					GetObjectID(),
					attackerType,
					attacker->GetObjectID()));

				GChat->AddText(attacker->GetObjectID(), format(L"{0} {1}��(��) ������",
					objectType,
					GetObjectID()));
			}
		}
	}
}

void Creature::KnockBack()
{
	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
