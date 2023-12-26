#include "pch.h"
#include "Player.h"
#include "Monster.h"
#include "Arrow.h"
#include "GameRoom.h"
#include "GameSession.h"

Player::Player()
{
	info.set_name("Player");
	info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	info.set_hp(60);
	info.set_maxhp(60);
	info.set_attack(20);
	info.set_defence(0);
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	switch (info.state())
	{
	case IDLE:
		UpdateIdle();
		break;

	case MOVE:
		UpdateMove();
		break;

	case SKILL:
		UpdateSkill();
		break;
	}
}

void Player::UpdateIdle()
{

}

void Player::UpdateMove()
{
	SetState(IDLE);
}

void Player::UpdateSkill()
{
	if (room == nullptr)
		return;

	if (info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
	{
		// 내 앞에 있는 좌표
		CreatureRef creature = room->GetCreatureAt(GetFrontCellPos());
		
		if (creature)
		{
			if (creature->GetType() == Protocol::OBJECT_TYPE_PLAYER)
			{
				SetState(IDLE);
				return;
			}
			// 몬스터가 플레이어에게 피격
			creature->OnDamaged(shared_from_this());
			/*creature->SetState(HIT, true);*/
		}
	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_BOW)
	{
		{
			// 화살 발사는 서버에서 관리
			ArrowRef arrow = CreateArrow();

			arrow->SetDir(shared_from_this()->info.dir());
			arrow->SetOwner(shared_from_this());

			arrow->room = room;
			arrow->info.set_posx(shared_from_this()->info.posx());
			arrow->info.set_posy(shared_from_this()->info.posy());
			arrow->SetState(IDLE);
			// arrow에 화살 id가 들어옴, 화살 패킷 전송
			{
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Fire(arrow->info, info.objectid());
				session->Send(sendBuffer);
				room->Broadcast(sendBuffer);
			}
			room->AddObject(arrow);
		}
	}
	SetState(IDLE);
}