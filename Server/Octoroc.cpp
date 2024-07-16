#include "pch.h"
#include "Octoroc.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"
#include "ProjectileRock.h"

Octoroc::Octoroc()
{
	info.set_name("Octoroc");
	info.set_monstertype(Protocol::MONSTER_TYPE_OCTOROC);
	info.set_hp(30);
	info.set_maxhp(30);
	info.set_attack(5);
	info.set_defence(0);
	info.set_speed(120);
}

Octoroc::~Octoroc()
{

}

void Octoroc::Init()
{
	Super::Init();
}

void Octoroc::Update()
{
	Super::Update();
}

void Octoroc::UpdateIdle()
{
	// �ٸ� ���Ϳ� �ٸ� ������ ��ã�� ����
	if (room == nullptr)
		return;

	// �ֺ��� �ִ� �÷��̾� ã��
	if (_target.lock() == nullptr)
		_target = room->FindClosestPlayer(GetCellPos());

	PlayerRef target = _target.lock();

	if (target)
	{
		// �÷��̾ ���� ���� ���� ���� ��� Ÿ�� ����
		if (target->IsInSafeZone())
		{
			_target.reset();
			return;
		}

		// ���� �Ǵ�
		Vec2Int dir = target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);

		// 5�� �Ÿ����� ����
		if ((dist <= 5 && dir.x == 0) || (dist <= 5 && dir.y == 0))
		{
			SetDir(GetLookAtDir(target->GetCellPos()));

			// �ֺ��� Ŭ���̾�Ʈ�� �˸�
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 3000; // 3�� ��ٸ�

			// ����ü �߻��ϱ�
			MakeRock();
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
			}
		}
	}
	else
	{
		// Ÿ���� ���� ��� ������ �������� ������
		int randValue = rand() % 4 + 1;
		switch (randValue)
		{
		case 1:
			SetDir(DIR_UP, true);
			break;

		case 2:
			SetDir(DIR_DOWN, true);
			break;

		case 3:
			SetDir(DIR_LEFT, true);
			break;

		case 4:
			SetDir(DIR_RIGHT, true);
			break;
		}

		if (room->MonsterCanGo(GetFrontCellPos()))
		{
			SetCellPos(GetFrontCellPos());
			_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

			// ������ Ŭ���̾�Ʈ�� �˸�
			SetState(MOVE, true);
		}

		// �Ÿ��� �ʹ� �־����� ���� �ڸ��� ����
		{
			auto pos = GetCellPos();

			auto dist = _initialPos - pos;
			auto len = dist.Length();

			if (len >= 10)
			{
				// ��ǥ ã��
				vector<Vec2Int> path;
				// ���� ��ġ�� �ǵ��ƴ� ���� ã��
				if (room->FindPath(GetCellPos(), _initialPos, OUT path))
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

							SetState(MOVE);
						}
					}
					else
						// ���� �� ã������ ����
					{
						SetCellPos(path[0]);
					}
				}
			}
		}
	}
}

void Octoroc::UpdateMove()
{
	Super::UpdateMove();
}

void Octoroc::UpdateSkill()
{
	Super::UpdateSkill();
}

void Octoroc::UpdateHit()
{
	Super::UpdateHit();
}

void Octoroc::MakeRock()
{
	// Ŭ���̾�Ʈ���Լ� ȭ�� ���� ��Ŷ ����
	ProjectileRockRef rock = CreateRock();

	rock->SetDir(shared_from_this()->info.dir());
	rock->SetOwner(shared_from_this());
	rock->room = room;
	rock->info.set_posx(shared_from_this()->info.posx());
	rock->info.set_posy(shared_from_this()->info.posy());
	rock->SetState(IDLE);
	room->AddObject(rock);

	uint64 id = GetObjectID();

	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Fire(rock->info, id);
	room->Broadcast(sendBuffer);
}