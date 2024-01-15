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
	info.set_mp(50);
	info.set_maxhp(5);
	info.set_maxmp(50);
	info.set_attack(20);
	info.set_defence(0);
	info.set_arrows(10);
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	_now = GetTickCount64();

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

	case TELEPORT:
		UpdateTeleport();
		break;
	}

	// 매 5초마다 플레이어의 mp를 회복시킴
	if (_now - _prev >= 5000)
	{
		info.set_mp(clamp(info.mp() + 5, 0, info.maxmp()));
		_prev = _now;
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
			creature->SetWait(50);
			creature->KnockBack(shared_from_this());
		}
	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_BOW)
	{

	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_STAFF)
	{
		
	}
}

void Player::UpdateTeleport()
{
	if (info.mp() >= 25)
	{
		Teleport();
		info.set_mp(clamp(info.mp() - 25, 0, info.maxmp()));
	}
	SetState(IDLE);
}

void Player::MakeArrow()
{
	if (info.arrows() <= 0)
		return;

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

	info.set_arrows(info.arrows() - 1);
}

void Player::Teleport()
{
	// 바라보는 방향 4칸 앞으로 이동 가능한지 체크(3칸 건너뛰기)
	Vec2Int fourAfterCellPos = { };
	Vec2Int _dir = {};

	switch (info.dir())
	{
	case DIR_UP:
		_dir = { 0, -1 };
		break;

	case DIR_DOWN:
		_dir = { 0, 1 };
		break;

	case DIR_LEFT:
		_dir = { -1, 0 };
		break;

	case DIR_RIGHT:
		_dir = { 1, 0 };
		break;
	}
	
	for (int i = 4; i >= 0; i--)
	{
		fourAfterCellPos = GetCellPos() + _dir * i;
		
		if (CanGo(fourAfterCellPos))
			break;
		else if (i == 0)
			return;
	}

	SetCellPos(fourAfterCellPos);
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(GetObjectID(), GetCellPos().x, GetCellPos().y);
		session->Send(sendBuffer);
		room->Broadcast(sendBuffer);
	}
}
