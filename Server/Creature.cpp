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

	// disable PvP : ������ ���� �Ұ�
	if (info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - info.defence();

	//if (damage <= 0)
	//	return;

	// hp�� �׻� ���
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
	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
