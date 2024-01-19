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
	info.set_attack(20);
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
	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	// ���� ��ġ�� �� �� �ִ��� Ȯ��
	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
		_waitUntil = GetTickCount64() + 50; // Ŭ���̾�Ʈ�� ȭ�� �ӵ��� ����ȭ
	}
	else
	{
		if (_target == _owner)
		{
			SetCellPos(nextPos + deltaXY[info.dir()]);
			SetState(MOVE);
			_waitUntil = GetTickCount64() + 50; // Ŭ���̾�Ʈ�� ȭ�� �ӵ��� ����ȭ
			return;
		}
		SetState(HIT);
		return;
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

	// ���� ��������� ����, ���Ͷ�� Ÿ��
	_target = dynamic_pointer_cast<Monster>(room->GetCreatureAt(nextPos));
	if (_target)
	{
		//_target->info.set_hp(_target->info.hp() + _target->info.defence() - shared_from_this()->info.attack());
		GChat->AddText(format(L"Player {0}��(��) �߻��� ȭ���� Monster {1}���� ����", _owner->GetObjectID(), _target->GetObjectID()));
		
		if (_target->info.hp() <= 0)
		{
			static_pointer_cast<Player>(_owner)->QuestProgress(0);
		}
	}
	else
	{
		GChat->AddText(format(L"Player {0}��(��) �߻��� ȭ���� ���� ����", _owner->GetObjectID()));
	}
	_hit = true;
}