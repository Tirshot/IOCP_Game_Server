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
	cout << "~Monster" << endl;
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
	// GameObject가 들고 있는 게임 룸을 체크
	if (room == nullptr)
		return;

	// 주변에 있는 플레이어 찾기
	if (_target.lock() == nullptr)
		_target = room->FindClosestPlayer(GetCellPos());

	PlayerRef target = _target.lock();

	if (target)
	{
		// 공격 판단
		Vec2Int dir = target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			// 공격하기
			SetDir(GetLookAtDir(target->GetCellPos()));

			// 몬스터가 공격한 패킷 전송
			//{
			//	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(target->GetObjectID(), GetObjectID());
			//	target->session->Send(sendBuffer);
			//	room->Broadcast(sendBuffer);
			//}

			// 주변의 클라이언트에 알림
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 1000; // 1초 기다림
		}
		else
		{
			// 좌표 찾기
			vector<Vec2Int> path;
			if (room->FindPath(GetCellPos(), target->GetCellPos(), OUT path))
			{
				if (path.size() > 1)
				{
					// 한 칸만 이동
					Vec2Int nextPos = path[1];
					if (room->CanGo(nextPos))
					{
						SetDir(GetLookAtDir(nextPos));
						SetCellPos(nextPos);
						_waitUntil = GetTickCount64() + 1000; // 1초 기다림

						// 주위의 클라이언트에 알림
						SetState(MOVE, true);
					}
				}
				else
					// 길을 못 찾았으면 정지
				{
					SetCellPos(path[0]);
				}
			}
			// 너무 멀어서 길을 찾지 못함
			else
			{
				_target.reset();
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

	SetState(IDLE);
}
