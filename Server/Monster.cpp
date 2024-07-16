#include "pch.h"
#include "Monster.h"
#include "GameRoom.h"
#include "Player.h"
#include "Item.h"
#include "Chat.h"
#include "GameSession.h"
#include "GameObject.h"
#include "ItemManager.h"

Monster::Monster()
{
	info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
	info.set_monstertype(Protocol::MONSTER_TYPE_NONE);
	info.set_hp(50);
	info.set_maxhp(50);
	info.set_attack(1);
	info.set_defence(0);
}

Monster::~Monster()
{
	_target.reset();
}

void Monster::Init()
{
	Super::BeginPlay();

	_initialPos = GetCellPos();
}

void Monster::Update()
{
	Super::Tick();

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
	Super::TickIdle();

	// GameObject가 들고 있는 게임 룸을 체크
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
		if (dist == 1)
		{
			SetDir(GetLookAtDir(target->GetCellPos()));

			// 주변의 클라이언트에 알림
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 1000; // 1초 기다림

			// 공격하기
			target->OnDamaged(shared_from_this());
			target->SetState(HIT, true);
			{
				int32 damage = info.attack() - target->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(target->info.objectid(), info.objectid(), damage);
				target->session->Send(sendBuffer);
			}
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
	else
	{
		// 타겟이 없을 경우 랜덤한 방향으로 움직임
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
			_waitUntil = GetTickCount64() + 1000; // 1초 기다림

			// 주위의 클라이언트에 알림
			SetState(MOVE, true);
		}

		// 타겟이 사라졌고 최초 소환 위치와 거리가 너무 멀어지면 복귀
		{
			auto pos = GetCellPos();

			auto dist = _initialPos - pos;
			auto len = dist.Length();

			if (len >= 10)
			{
				// 좌표 찾기
				vector<Vec2Int> path;
				// 최초 위치로 되돌아가는 길을 찾음
				if (room->FindPath(GetCellPos(), _initialPos, OUT path))
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

							SetState(MOVE);
						}
					}
					else
						// 길을 못 찾았으면 정지
					{
						SetCellPos(path[0]);
					}
				}
			}
		}
	}
}

void Monster::UpdateMove()
{
	Super::TickMove();

	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateSkill()
{
	Super::TickSkill();

	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateHit()
{
	Super::TickHit();

	uint64 now = GetTickCount64();

	if (_waitHit > now)
		return;

	SetState(IDLE);
}

void Monster::ItemDrop(CreatureRef owner)
{
	// 몬스터 타입에 따른 아이템 드랍 그룹 테이블 가져오기
	auto monsterType = info.monstertype();
	auto groupInfo = GET_SINGLE(ItemManager)->GetMonsterDropItemGroupInfo(monsterType);

	if (groupInfo.empty())
		return;

	// 그룹 테이블에서 아이템 그룹 별 확률 가져오기
	int WearableProbablity = GET_SINGLE(ItemManager)->GetWearableProbablity(groupInfo);
	int ConsumableProbablity = WearableProbablity + GET_SINGLE(ItemManager)->GetConsumableProbablity(groupInfo);
	int ETCProbablity = ConsumableProbablity + GET_SINGLE(ItemManager)->GetETCProbablity(groupInfo);
	int NoneProbablity = ETCProbablity + GET_SINGLE(ItemManager)->GetNoneProbablity(groupInfo);

	// 주사위 던지기
	int randValue = rand() % 100 + 1; // 1 ~ 100

	// 해당 몬스터 타입에 유효한 개별 아이템 드랍 테이블 가져오기
	auto dropTable = GET_SINGLE(ItemManager)->GetItemDropTableByMonsterType(monsterType);

	vector<vector<wstring>> group;

	// 그룹 확률에 따라 아이템 드랍
	if (randValue <= WearableProbablity)
	{
		// 장비 아이템 드랍
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"Wearable");
	}
	else if (randValue > WearableProbablity && randValue <= ConsumableProbablity)
	{
		// 소모품 아이템 드랍
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"Consumable");
	}
	else if (randValue > ConsumableProbablity && randValue <= ETCProbablity)
	{
		// 기타 아이템 드랍 - 퀘스트 아이템 등
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"ETC");
	}
	else if (randValue > ETCProbablity && randValue <= NoneProbablity)
	{
		// 아무 아이템도 드랍되지 않음
		return;
	}

	if (group.size() == 0)
	{
		GChat->AddText(L"<ERROR> 드랍 테이블을 찾을 수 없음. 몬스터 별 전리품(ETC)이 있는지 확인.");
		return;
	}

	// 아이템 생성, 패킷 전송
	auto item = GET_SINGLE(ItemManager)->MakeItem(group, owner, {GetCellPos().x, GetCellPos().y});
}

void Monster::GoldDrop(CreatureRef owner)
{
	// 그룹 테이블에서 최소 골드, 최대 골드를 추출
	auto itemGroupinfo = GET_SINGLE(ItemManager)->GetMonsterDropItemGroupInfo(info.monstertype());
	int minGold = GET_SINGLE(ItemManager)->GetMinGold(itemGroupinfo);
	int maxGold = GET_SINGLE(ItemManager)->GetMaxGold(itemGroupinfo);

	// 최대와 최소의 차를 통해 몇 개의 기대값이 있는지 추출
	int step = maxGold - minGold;

	// 최소 ~ 최대 사이의 값을 추출
	int randValue = rand() % step + 1;

	int gold = minGold + randValue;

	// 골드 생성 후 패킷 전송
	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Gold(owner->GetObjectID(), gold);
	PlayerRef player = static_pointer_cast<Player>(GRoom->FindObject(owner->GetObjectID()));

	if (player)
	{
		player->session->Send(sendBuffer);
		GChat->AddText(format(L"Player{0}, {1} gold 획득.", owner->GetObjectID(), gold));
	}
}

void Monster::OnDamaged(CreatureRef attacker, bool debug)
{
	Super::OnDamaged(attacker);

	// 피격 후 타겟 전환

	// 타겟이 없는 경우
	if (_target.expired())
	{
		auto player = dynamic_pointer_cast<Player>(attacker);
		if (player)
		{
			_target = player;
			_waitUntil = GetTickCount64() + 1000; // 1초 기다림
		}

	}
	else if (_target.lock() != attacker)
	{
		_target.reset();

		auto player = dynamic_pointer_cast<Player>(attacker);
		if (player)
		{
			_target = player;
			_waitUntil = GetTickCount64() + 1000; // 1초 기다림
		}
	}

	// 인 게임 디버그 커맨드로 몬스터가 처치당했을 때
	if (debug)
	{
		// 사망 처리
		if (room)
		{
			room->RemoveObject(GetObjectID());

			PlayerRef player = dynamic_pointer_cast<Player>(attacker);
			if (player)
			{
				MonsterQuestProgress(player);
			}

			// 채팅 출력
			GChat->AddText(format(L"debug : {0} {1}이(가) {2}에 의해 처치됨.",
				GetName(),
				GetObjectID(),
				attacker->GetName()));

			// 아이템 드랍
			ItemDrop(attacker);
			GoldDrop(attacker);

			return;
		}
	}

	if (info.hp() <= 0)
	{
		// 사망 처리
		if (room)
		{
			room->RemoveObject(GetObjectID());

			PlayerRef player = dynamic_pointer_cast<Player>(attacker);
			if (player)
			{
				MonsterQuestProgress(player);
			}

			// 채팅 출력
			GChat->AddText(format(L"{0} {1}이(가) {2}에 의해 처치됨.",
				GetName(),
				GetObjectID(),
				attacker->GetName()));

			// 아이템 드랍
			ItemDrop(attacker);
			GoldDrop(attacker);
		}
	}
}

wstring Monster::GetName()
{
	return GET_SINGLE(ItemManager)->StringToWString(info.name());
}

void Monster::KnockBack()
{
	Super::KnockBack();

	// 몬스터의 경우 넉백당하면 타겟을 리셋
	_target.reset();
}

void Monster::MonsterQuestProgress(PlayerRef player)
{
	player->KillQuestProgress(info.monstertype());
}
