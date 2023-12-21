#include "pch.h"
#include "Monster.h"
#include "GameRoom.h"
#include "Player.h"

Monster::Monster()
{
	info.set_name("MonsterName");
	info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
	info.set_hp(50);
	info.set_maxhp(50);
	info.set_attack(10);
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
					if (room->CanGo(nextPos))
					{
						SetDir(GetLookAtDir(nextPos));
						SetCellPos(nextPos);
						_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

						// ������ Ŭ���̾�Ʈ�� �˸�
						SetState(MOVE, true);
					}
				}
				else
					// ���� �� ã������ ����, Ÿ�� ����
				{
					SetCellPos(path[0]);
					_target.reset();
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
