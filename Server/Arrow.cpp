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

	// 다음 위치에 갈 수 있는지 확인
	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
		_waitUntil = GetTickCount64() + 50; // 클라이언트의 화살 속도와 동기화
	}
	else
	{
		if (_target == _owner)
		{
			SetCellPos(nextPos + deltaXY[info.dir()]);
			SetState(MOVE);
			_waitUntil = GetTickCount64() + 50; // 클라이언트의 화살 속도와 동기화
		}

		SetState(HIT);
	}
	_hit = false;
}

void Arrow::TickMove()
{
	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

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
 		GChat->AddText(format(L"Player {0}이(가) 발사한 화살이 Monster {1}에게 적중", _owner->GetObjectID(), _target->GetObjectID()));
	}
	else
	{
		GChat->AddText(format(L"Player {0}이(가) 발사한 화살이 벽에 맞음", _owner->GetObjectID()));
	}
	_hit = true;
}