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

	int32 damage = attacker->info.attack() - info.defence();

	//if (damage <= 0)
	//	return;

	// hp는 항상 양수
 	info.set_hp(max(0, info.hp() - damage));
	// //Hit Packet
	//{
	//	Protocol::S_Hit pkt;
	//	pkt.set_objectid(GetObjectID());
	//	pkt.set_attackerid(attacker->GetObjectID());
	//	pkt.set_objecttype(info.objecttype());

	//	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(pkt);
	//	room->Broadcast(sendBuffer);
	//}

	if (info.hp() == 0)
	{
		if (room)
		{
			room->RemoveObject(GetObjectID());
		}
	}

}

void Creature::KnockBack()
{
	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
