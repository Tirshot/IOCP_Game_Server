#include "pch.h"
#include "Player.h"
#include "Monster.h"
#include "Arrow.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Chat.h"

Player::Player()
{
	info.set_name("Player");
	info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	info.set_hp(5);
	info.set_maxhp(5);
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
	SetState(MOVE);
	SetState(IDLE);
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
			creature->SetState(HIT); // 서버 사이드의 상태 변경이 필요한가? 이미 클라에서 변경되었음
			//creature->OnDamaged(shared_from_this());
		}
	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_BOW)
	{
		
	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_STAFF)
	{

	}
	SetState(IDLE);
}

void Player::MakeArrow()
{
	// 클라이언트에게서 화살 생성 패킷 받음
	ArrowRef arrow = CreateArrow();

	arrow->SetDir(shared_from_this()->info.dir());
	arrow->SetOwner(shared_from_this());

	arrow->room = room;
	arrow->info.set_posx(shared_from_this()->info.posx());
	arrow->info.set_posy(shared_from_this()->info.posy());
	arrow->SetState(IDLE);
	// 클라이언트로 화살 생성 패킷 전송
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Fire(arrow->info, info.objectid());
		session->Send(sendBuffer);
		room->Broadcast(sendBuffer);
	}
	room->AddObject(arrow);
	uint64 ownerid = arrow->GetOwner()->GetObjectID();
}

void Player::Teleport()
{
	// 바라보는 방향 4칸 앞으로 이동 가능한지 체크(3칸 건너뛰기)
	Vec2Int fiveAfterCellPos = {};

	switch (info.dir())
	{
	case Protocol::DIR_TYPE_UP:
		fiveAfterCellPos = GetFrontCellPos() + Vec2Int{ 0, -4 };
		break;

	case Protocol::DIR_TYPE_DOWN:
		fiveAfterCellPos = GetFrontCellPos() + Vec2Int{ 0, 4 };
		break;

	case  Protocol::DIR_TYPE_LEFT:
		fiveAfterCellPos = GetFrontCellPos() + Vec2Int{ -4, 0 };
		break;

	case  Protocol::DIR_TYPE_RIGHT:
		fiveAfterCellPos = GetFrontCellPos() + Vec2Int{ 4, 0 };
		break;

	default:
		return;
	}

	if (CanGo(fiveAfterCellPos))
	{
		SetCellPos(fiveAfterCellPos);

		{ // 패킷 전송
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(GetObjectID(), fiveAfterCellPos.x, fiveAfterCellPos.y);
			session->Send(sendBuffer);
		}
	}
	else
	{
		while (!CanGo(fiveAfterCellPos))
		{	// 계산한 값에서 한칸씩 빼면서 이동가능한지 확인
			switch (info.dir())
			{
			case Protocol::DIR_TYPE_UP:
				fiveAfterCellPos += Vec2Int{ 0, 1 };
				break;

			case Protocol::DIR_TYPE_DOWN:
				fiveAfterCellPos += Vec2Int{ 0, -1 };
				break;

			case Protocol::DIR_TYPE_LEFT:
				fiveAfterCellPos += Vec2Int{ 1, 0 };
				break;

			case Protocol::DIR_TYPE_RIGHT:
				fiveAfterCellPos += Vec2Int{ -1, 0 };
				break;

			default:
				return;
			}
			if (fiveAfterCellPos == GetCellPos())
			{
				SetState(IDLE);
				return;
			}
		}
		// 이동
		SetCellPos(fiveAfterCellPos);

		{ // 패킷 전송
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(info.objectid(), fiveAfterCellPos.x, fiveAfterCellPos.y);
			session->Send(sendBuffer);
		}
	}
}
