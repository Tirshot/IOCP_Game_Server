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
		// �� �տ� �ִ� ��ǥ
		CreatureRef creature = room->GetCreatureAt(GetFrontCellPos());
		
		if (creature)
		{
			if (creature->GetType() == Protocol::OBJECT_TYPE_PLAYER)
			{
				SetState(IDLE);
				return;
			}
			// ���Ͱ� �÷��̾�� �ǰ�
			creature->SetState(HIT); // ���� ���̵��� ���� ������ �ʿ��Ѱ�? �̹� Ŭ�󿡼� ����Ǿ���
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
	// Ŭ���̾�Ʈ���Լ� ȭ�� ���� ��Ŷ ����
	ArrowRef arrow = CreateArrow();

	arrow->SetDir(shared_from_this()->info.dir());
	arrow->SetOwner(shared_from_this());

	arrow->room = room;
	arrow->info.set_posx(shared_from_this()->info.posx());
	arrow->info.set_posy(shared_from_this()->info.posy());
	arrow->SetState(IDLE);
	// Ŭ���̾�Ʈ�� ȭ�� ���� ��Ŷ ����
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
	// �ٶ󺸴� ���� 4ĭ ������ �̵� �������� üũ(3ĭ �ǳʶٱ�)
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

		{ // ��Ŷ ����
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(GetObjectID(), fiveAfterCellPos.x, fiveAfterCellPos.y);
			session->Send(sendBuffer);
		}
	}
	else
	{
		while (!CanGo(fiveAfterCellPos))
		{	// ����� ������ ��ĭ�� ���鼭 �̵��������� Ȯ��
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
		// �̵�
		SetCellPos(fiveAfterCellPos);

		{ // ��Ŷ ����
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(info.objectid(), fiveAfterCellPos.x, fiveAfterCellPos.y);
			session->Send(sendBuffer);
		}
	}
}
