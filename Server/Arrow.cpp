#include "pch.h"
#include "Arrow.h"
#include "GameRoom.h"
#include "Creature.h"
#include "Monster.h"
#include "Player.h"
#include "ServerPacketHandler.h"
#include "GameSession.h"
#include "Chat.h"

Arrow::Arrow()
{
	info.set_attack(10);
	info.set_objecttype(Protocol::OBJECT_TYPE_PROJECTILE);
}

Arrow::~Arrow()
{

}

void Arrow::BeginPlay()
{
	
}

void Arrow::Tick()
{
	if (this == nullptr)
		return;

	switch (info.state())
	{
	case IDLE:
		TickIdle();
		break;

	case MOVE:
		TickMove();
		break;

	case HIT:
		TickHit();
		break;
	}
}

void Arrow::TickIdle()
{
	if (room == nullptr)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	float tick = (float)(GetTickCount64()) / 1000.0f;
	if (_waitUntil <= 0.0f)
	{
		_waitUntil = tick + 0.05f;
	}

	if (tick < _waitUntil)
		return;

	_waitUntil = tick + 0.05f;

	// 다음 위치에 갈 수 있는지 확인
	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
	}
	else
	{
		if (_target == _owner)
		{
			SetCellPos(nextPos);
			SetState(MOVE);
			return;
		}
		SetState(HIT);
		return;
	}

	_waitUntil = 0.f;
	_hit = false;
}

void Arrow::TickMove()
{
	SetState(IDLE);
}

void Arrow::TickHit()
{
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int pos = Vec2Int{ info.posx(), info.posy() };
	Vec2Int nextPos = pos + deltaXY[info.dir()];

	// 앞이 비어있으면 전진, 몬스터라면 타격
	_target = dynamic_pointer_cast<Monster>(room->GetCreatureAt(nextPos));
	if (_target)
	{
		int damage = _owner->info.attack() - _target->info.defence();

		_target->OnDamaged(_owner);
		{
			int32 damage = info.attack() - _target->info.defence();
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(_target->info.objectid(), _owner->info.objectid(), damage);
			
			PlayerRef player = dynamic_pointer_cast<Player>(_owner);
			player->session->Send(sendBuffer);
		}

		GChat->AddText(format(L"Player {0}이(가) 발사한 화살이 Monster {1}에게 적중", _owner->GetObjectID(), _target->GetObjectID()));
	}
	else
	{
		GChat->AddText(format(L"Player {0}이(가) 발사한 화살이 벽에 맞음", _owner->GetObjectID()));
	}
	_hit = true;
}