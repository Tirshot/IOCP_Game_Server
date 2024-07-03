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
	info.set_attack(10);
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
	// 다른 몬스터와 다른 고유의 길찾기 수행
	if (room == nullptr)
		return;

	// 주변에 있는 플레이어 찾기
	if (_target.lock() == nullptr)
		_target = room->FindClosestPlayer(GetCellPos());

	PlayerRef target = _target.lock();

	if (target)
	{
		// 플레이어가 안전 구역 내에 있을 경우 타겟 해제
		if (target->IsInSafeZone())
		{
			_target.reset();
			return;
		}

		// 공격 판단
		Vec2Int dir = target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);

		// 5의 거리에서 공격
		if ((dist <= 5 && dir.x == 0) || (dist <= 5 && dir.y == 0))
		{
			SetDir(GetLookAtDir(target->GetCellPos()));

			// 주변의 클라이언트에 알림
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 3000; // 3초 기다림

			// 투사체 발사하기
			MakeRock();
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
					if (room->MonsterCanGo(nextPos))
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
	// 클라이언트에게서 화살 생성 패킷 받음
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