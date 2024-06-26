#include "pch.h"
#include "ProjectileRock.h"
#include "Monster.h"
#include "Player.h"
#include "Chat.h"
#include "GameRoom.h"
#include "GameSession.h"

ProjectileRock::ProjectileRock()
{
}

ProjectileRock::~ProjectileRock()
{
}

void ProjectileRock::BeginPlay()
{
}

void ProjectileRock::Tick()
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

void ProjectileRock::TickIdle()
{
	if (room == nullptr)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	float tick = (float)(GetTickCount64()) / 1000.0f;
	if (_waitUntil <= 0.0f)
	{
		_waitUntil = tick + 0.062f;
	}

	if (tick < _waitUntil)
		return;

	_waitUntil = tick + 0.062f;

	// ���� ��ġ�� �� �� �ִ��� Ȯ��
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

void ProjectileRock::TickMove()
{
	SetState(IDLE);
}

void ProjectileRock::TickHit()
{
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int pos = Vec2Int{ info.posx(), info.posy() };
	Vec2Int nextPos = pos + deltaXY[info.dir()];

	// ���� ��������� ����, ���Ͷ�� Ÿ��
	_target = dynamic_pointer_cast<Player>(room->GetCreatureAt(nextPos));
	if (_target)
	{
		int damage = _owner->info.attack() - _target->info.defence();
		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(_target->info.objectid(), _owner->info.objectid(), damage);
			GRoom->Broadcast(sendBuffer);
		}
		_target->OnDamaged(_owner);

		GChat->AddText(format(L"Octoroc {0}��(��) �߻��� ���� Player {1}���� ����", _owner->GetObjectID(), _target->GetObjectID()));
	}
	else
	{
		GChat->AddText(format(L"Octoroc {0}��(��) �߻��� ���� ���� ����", _owner->GetObjectID()));
	}
	_hit = true;
}
