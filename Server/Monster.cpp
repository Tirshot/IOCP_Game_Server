#include "pch.h"
#include "Monster.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameSession.h"

Monster::Monster()
{
	info.set_name("Snake");
	info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
	info.set_hp(50);
	info.set_maxhp(50);
	info.set_attack(1);
	info.set_defence(0);
}

Monster::~Monster()
{

}

void Monster::Init()
{

}

void Monster::Update()
{
	// Super::Update();

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

	case HIT:
		UpdateHit();
		break;
	}
}

void Monster::UpdateIdle()
{
	// GameObject�� ��� �ִ� ���� ���� üũ
	if (room == nullptr)
		return;

	// �ֺ��� �ִ� �÷��̾� ã��
	if (_target.lock() == nullptr)
		_target = room->FindClosestPlayer(GetCellPos());

	PlayerRef target = _target.lock();

	if (target)
	{
		// ���� �Ǵ�
		Vec2Int dir = target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			// �����ϱ�
			SetDir(GetLookAtDir(target->GetCellPos()));

			// �ֺ��� Ŭ���̾�Ʈ�� �˸�
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�
		}
		else
		{
			// ��ǥ ã��
			vector<Vec2Int> path;
			if (room->FindPath(GetCellPos(), target->GetCellPos(), OUT path))
			{
				if (path.size() > 1)
				{
					// �� ĭ�� �̵�
					Vec2Int nextPos = path[1];
					if (room->MonsterCanGo(nextPos))
					{
						SetDir(GetLookAtDir(nextPos));
						SetCellPos(nextPos);
						_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

						// ������ Ŭ���̾�Ʈ�� �˸�
						SetState(MOVE, true);
					}
				}
				else
					// ���� �� ã������ ����
				{
					SetCellPos(path[0]);
				}
			}
			// �ʹ� �־ ���� ã�� ����
			else
			{
				_target.reset();

				{ // Ÿ���� ã�� ���ϸ� �������� �� ĭ�� �̵�
					// 0, 1, 2, 3
					auto randPos = rand() % 3;
					Vec2Int nextPos = {};

					switch (randPos)
					{
					case DIR_UP:
						nextPos = GetCellPos() + Vec2Int{ 0, -1 };

						if (room->MonsterCanGo(nextPos))
						{
							SetDir(GetLookAtDir(nextPos));
							SetCellPos(nextPos);
							_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

							// ������ Ŭ���̾�Ʈ�� �˸�
							SetState(MOVE, true);
						}
						break;

					case DIR_DOWN:
						nextPos = GetCellPos() + Vec2Int{ 0, 1 };

						if (room->MonsterCanGo(nextPos))
						{
							SetDir(GetLookAtDir(nextPos));
							SetCellPos(nextPos);
							_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

							// ������ Ŭ���̾�Ʈ�� �˸�
							SetState(MOVE, true);
						}
						break;

					case DIR_LEFT:
						nextPos = GetCellPos() + Vec2Int{ -1, 0 };

						if (room->MonsterCanGo(nextPos))
						{
							SetDir(GetLookAtDir(nextPos));
							SetCellPos(nextPos);
							_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

							// ������ Ŭ���̾�Ʈ�� �˸�
							SetState(MOVE, true);
						}
						break;

					case DIR_RIGHT:
						nextPos = GetCellPos() + Vec2Int{ 1, 0 };

						if (room->MonsterCanGo(nextPos))
						{
							SetDir(GetLookAtDir(nextPos));
							SetCellPos(nextPos);
							_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

							// ������ Ŭ���̾�Ʈ�� �˸�
							SetState(MOVE, true);
						}
						break;
					}
				}
			}
		}
	}
}

void Monster::UpdateMove()
{
	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateSkill()
{
	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateHit()
{
	uint64 now = GetTickCount64();

	if (_waitHit > now)
		return;

	SetState(IDLE);
}
