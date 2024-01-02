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

	// disable PvP : 동족은 공격 불가
	if (info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	//// hp는 항상 양수
 //	info.set_hp(max(0, info.hp() - damage));

	// 채팅 출력
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

		GChat->AddText(format(L"{0} {1}이(가) {2} {3}에게 {4}의 피해를 입힘",
			attackerType,
			attacker->GetObjectID(),
			objectType,
			GetObjectID(),
			damage));

		GChat->AddText(attacker->GetObjectID(), format(L"{0} {1}에게 {2}의 피해를 입힘",
			objectType,
			GetObjectID(),
			damage));

		if (info.hp() == 0)
		{
			if (room)
			{
				room->RemoveObject(GetObjectID());
				GChat->AddText(format(L"{0} {1}이(가) {2} {3}에 의해 쓰러짐",
					objectType,
					GetObjectID(),
					attackerType,
					attacker->GetObjectID()));

				GChat->AddText(attacker->GetObjectID(), format(L"{0} {1}이(가) 쓰러짐",
					objectType,
					GetObjectID()));
			}
		}
	}
}

void Creature::KnockBack()
{
	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
