#include "pch.h"
#include "Arrow.h"
#include "GameRoom.h"
#include "Creature.h"
#include "Monster.h"
#include "Player.h"
#include "ServerPacketHandler.h"
#include "GameSession.h"

Arrow::Arrow()
{
	cout << "Arrow" << endl;
}

Arrow::~Arrow()
{
	cout << "~Arrow" << endl;
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
	/*Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];*/
	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	// 다음 위치에 갈 수 있는지 확인
	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
		_waitUntil = GetTickCount64() + 50; // 기다림
		_hit = false;
	}
	else
	{
		SetState(HIT);
	}
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
	/*Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];*/
	Vec2Int pos = Vec2Int{ info.posx(), info.posy() };
	Vec2Int nextPos = pos + deltaXY[info.dir()];

	// 앞이 비어있으면 전진, 몬스터라면 타격
	_target = static_pointer_cast<Monster>(room->GetCreatureAt(nextPos));
	if (_target)
	{
		_target->OnDamaged(_owner);
		cout << "몬스터에게 적중" << endl;
	}
	else
	{
		cout << "벽에 적중" << endl;
	}
	_hit = true;
}
