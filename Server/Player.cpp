#include "pch.h"
#include "Player.h"
#include "Monster.h"
#include "Arrow.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Chat.h"

Player::Player()
{
	info.set_name("Player");
	info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	info.set_hp(50);
	info.set_mp(50);
	info.set_maxhp(50);
	info.set_maxmp(50);
	info.set_attack(10);
	info.set_defence(0);
	info.set_arrows(10);
	info.set_gold(1500);
	info.set_speed(180);
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	_now = GetTickCount64();

	switch (info.state())
	{
	case IDLE:
		UpdateIdle();
		break;

	case MOVE:
		UpdateMove();
		break;

	default:
		break;
	}

	// 안전 구역에서 스킬 사용 불가
	auto player = dynamic_pointer_cast<Player>(shared_from_this());
	if (player)
	{
		if (IsInSafeZone())
		{
			SetState(IDLE, true);
			return;
		}
	}

	switch (info.state())
	{
	case SKILL:
		UpdateSkill();
		break;

	case HIT:
		UpdateHit();
		break;

	case SPIN:
		UpdateSpin();
		break;

	case TELEPORT:
		UpdateTeleport();
		break;

	default:
		return;
	}

	// 매 1.5초마다 플레이어의 mp를 회복시킴
	if (_now - _prev >= 1500 && info.mp() < info.maxmp())
	{
		info.set_mp(clamp(info.mp() + 5, 0, info.maxmp()));

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MPRecover(info.objectid(), info.mp());
		session->Send(sendBuffer);

		_prev = _now;
	}
}

void Player::UpdateIdle()
{
}

void Player::UpdateMove()
{
	SetState(IDLE);
}

void Player::UpdateSkill()
{
	if (room == nullptr)
		return;

	if (info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
	{
		// 내 앞에 있는 좌표
		CreatureRef creature = room->GetCreatureAt(GetFrontCellPos());

		if (creature)
		{
			if (creature->GetType() != Protocol::OBJECT_TYPE_MONSTER)
			{
				SetState(IDLE);
				return;
			}
			// 몬스터가 플레이어에게 피격
			creature->OnDamaged(shared_from_this());

			int damage = shared_from_this()->info.attack() - creature->info.defence();
			{
				int32 damage = info.attack() - creature->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(creature->info.objectid(), info.objectid(), damage);
				session->Send(sendBuffer);
			}

			if (creature->info.hp() <= 0)
				return;

			creature->SetWait(50);
		}
	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_BOW)
	{

	}
	else if (info.weapontype() == Protocol::WEAPON_TYPE_STAFF)
	{
		
	}
	SetState(IDLE);
}

void Player::UpdateHit()
{
	SetState(IDLE);
}

void Player::UpdateSpin()
{
	if (info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
	{
		// 내 십자에 있는 좌표
		CreatureRef creature = room->GetCreatureAt(GetCellPos() + Vec2Int{ 0,-1 }); // up
		CreatureRef creature2 = room->GetCreatureAt(GetCellPos() + Vec2Int{ 1,0 }); // right
		CreatureRef creature3 = room->GetCreatureAt(GetCellPos() + Vec2Int{ 0,1 }); // down
		CreatureRef creature4 = room->GetCreatureAt(GetCellPos() + Vec2Int{ -1,0 }); // left

		if (creature)
		{
			if (creature->GetType() != Protocol::OBJECT_TYPE_MONSTER)
			{
				SetState(IDLE);
				return;
			}
			creature->OnDamaged(shared_from_this());

			int damage = shared_from_this()->info.attack() - creature->info.defence();
			{
				int32 damage = info.attack() - creature->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(creature->info.objectid(), info.objectid(), damage);
				session->Send(sendBuffer);
			}
			creature->SetWait(50);
		}

		if (creature2)
		{
			if (creature2->GetType() != Protocol::OBJECT_TYPE_MONSTER)
			{
				SetState(IDLE);
				return;
			}
			creature2->OnDamaged(shared_from_this());

			int damage = shared_from_this()->info.attack() - creature2->info.defence();
			{
				int32 damage = info.attack() - creature2->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(creature2->info.objectid(), info.objectid(), damage);
				session->Send(sendBuffer);
			}
			creature2->SetWait(50);
		}

		if (creature3)
		{
			if (creature3->GetType() != Protocol::OBJECT_TYPE_MONSTER)
			{
				SetState(IDLE);
				return;
			}
			creature3->OnDamaged(shared_from_this());

			int damage = shared_from_this()->info.attack() - creature3->info.defence();
			{
				int32 damage = info.attack() - creature3->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(creature3->info.objectid(), info.objectid(), damage);
				session->Send(sendBuffer);
			}
			creature3->SetWait(50);
		}

		if (creature4)
		{
			if (creature4->GetType() != Protocol::OBJECT_TYPE_MONSTER)
			{
				SetState(IDLE);
				return;
			}

			creature4->OnDamaged(shared_from_this());

			int damage = shared_from_this()->info.attack() - creature4->info.defence();
			{
				int32 damage = info.attack() - creature4->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(creature4->info.objectid(), info.objectid(), damage);
				session->Send(sendBuffer);
			}
			creature4->SetWait(50);
		}
	}
	SetState(MOVE);
}

void Player::UpdateTeleport()
{
	if (info.mp() >= 25)
	{
		Teleport();
		info.set_mp(clamp(info.mp() - 25, 0, info.maxmp()));
	}
	SetState(IDLE);
}

void Player::OnDamaged(CreatureRef attacker, bool debug)
{
	Super::OnDamaged(attacker);

	if (info.hp() <= 0)
	{
		if (room)
		{
			room->RemoveObject(GetObjectID());

			wstring objectType = GChat->StringToWStr(info.name());
			wstring attackerType = GChat->StringToWStr(attacker->info.name());

			// 채팅 출력
			GChat->AddText(format(L"{0} {1}이(가) {2} {3}에 의해 쓰러짐",
				objectType,
				GetObjectID(),
				attackerType,
				attacker->GetObjectID()));

			if (debug)
			{
				GChat->AddText(L"DEBUG : Player Kill");
			}
		}
	}
}

void Player::MakeArrow()
{
	if (info.arrows() <= 0)
		return;

	// 클라이언트에게서 화살 생성 패킷 받음
	ArrowRef arrow = CreateArrow();

	arrow->SetDir(shared_from_this()->info.dir());
	arrow->SetOwner(shared_from_this());

	arrow->room = room;
	arrow->info.set_posx(shared_from_this()->info.posx());
	arrow->info.set_posy(shared_from_this()->info.posy());
	arrow->SetState(IDLE);
	room->AddObject(arrow);

	uint64 id = GetObjectID();

	auto arrows = info.arrows();
	info.set_arrows(arrows - 1);
	
	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Fire(arrow->info, id);
	session->Send(sendBuffer);
	room->Broadcast(sendBuffer, id);
}

void Player::Teleport()
{
	// 바라보는 방향 4칸 앞으로 이동 가능한지 체크(3칸 건너뛰기)
	Vec2Int fourAfterCellPos = { };
	Vec2Int _dir = {};

	switch (info.dir())
	{
	case DIR_UP:
		_dir = { 0, -1 };
		break;

	case DIR_DOWN:
		_dir = { 0, 1 };
		break;

	case DIR_LEFT:
		_dir = { -1, 0 };
		break;

	case DIR_RIGHT:
		_dir = { 1, 0 };
		break;
	}
	
	for (int i = 4; i >= 0; i--)
	{
		fourAfterCellPos = GetCellPos() + _dir * i;
		
		if (CanGo(fourAfterCellPos))
			break;
		else if (i == 0)
			return;
	}

	SetCellPos(fourAfterCellPos);
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Teleport(GetObjectID(), GetCellPos().x, GetCellPos().y);
		session->Send(sendBuffer);
		room->Broadcast(sendBuffer);
	}
}

void Player::QuestProgress(int questid)
{
	auto questsStates = room->GetQuestsStates(GetObjectID(), questid);

	Protocol::QUEST_STATE state = questsStates.state;
	Protocol::QuestInfo questInfo = room->GetQuest(questid);
	int targetNums = questInfo.targetnums();
	int newProgress = questsStates.progress + 1;

	if (state == Protocol::QUEST_STATE_FINISHED || state == Protocol::QUEST_STATE_COMPLETED)
		return;

	if (state == Protocol::QUEST_STATE_ACCEPT)
	{
		// 몬스터 처치 카운트 
		{
			room->SetQuestStateProgress(GetObjectID(), questid, newProgress);
			// newProgress ++
			{
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_QuestProcess(GetObjectID(), questid, newProgress);
				session->Send(sendBuffer);
			}

			if (newProgress >= targetNums)
			{
				{
					SendBufferRef sendBuffer = ServerPacketHandler::Make_S_QuestComplete(GetObjectID(), questid, newProgress);
					session->Send(sendBuffer);
					room->SetQuestStates(GetObjectID(), questid, Protocol::QUEST_STATE_COMPLETED);
					return;
				}
			}
		}
	}
	return;
}

map<int, PlayerQuestState> Player::GetAcceptedQuests()
{
	map<int, PlayerQuestState> playerQuestsStates;

	int objectId = GetObjectID();
	auto questsStates = GRoom->GetQuestsStates(objectId);
	for (auto& questState : questsStates)
	{
		if (questState.second.state == Protocol::QUEST_STATE_ACCEPT)
		{
			playerQuestsStates.insert(questState);
		}
	}

	return playerQuestsStates;
}

Protocol::QUEST_STATE Player::GetQuestState(int questId)
{
	auto questState = GRoom->GetQuestsStates(GetObjectID(), questId);

	return questState.state;
}
