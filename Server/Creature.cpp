#include "pch.h"
#include "Creature.h"
#include "GameRoom.h"

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

	// disable PvP : 동족은 공격 불가
	if (info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - this->info.defence();
	attacker->info.set_damage(damage);

	if (damage <= 0)
		return;

	// hp는 항상 양수
	info.set_hp(max(0, info.hp() - damage));

	// 입힌 데미지를 브로드캐스트
	BroadcastMove();

	if (info.hp() == 0)
	{
		if (room)
		{
			room->RemoveObject(GetObjectID());
		}
	}

}
