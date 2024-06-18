#include "pch.h"
#include "Creature.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Chat.h"
#include "Player.h"
#include "Item.h"
#include "ItemManager.h"

Creature::Creature()
{

}

Creature::~Creature()
{

}

void Creature::BeginPlay()
{
	Super::BeginPlay();
}

void Creature::Tick()
{
	Super::Tick();
}

void Creature::OnDamaged(CreatureRef attacker, bool debug)
{
	if (attacker == nullptr)
		return;

	if (shared_from_this() == nullptr)
		return;

	// disable PvP : 동족은 공격 불가
	if (info.objecttype() == attacker->info.objecttype())
		return;

	// NPC 공격 불가
	if (info.objecttype() == Protocol::OBJECT_TYPE_NPC || info.objecttype() == Protocol::OBJECT_TYPE_NPC_SIGN)
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	// hp는 항상 양수
 	info.set_hp(max(0, info.hp() - damage));

	if (info.hp() <= 0)
		info.set_hp(0);

	// 채팅 출력
	{
		wstring attackerType = L"";
		wstring objectType = L"";

		if (attacker->info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
		{
			attackerType = L"Player";
			objectType = L"Monster";
		}
		else
		{
			attackerType = L"Monster";
			objectType = L"Player";
		}

		GChat->AddText(format(L"{0} {1}이(가) {2} {3}에게 {4}의 피해를 입힘",
			attackerType,
			attacker->GetObjectID(),
			objectType,
			GetObjectID(),
			damage));
	}
	// 사망 로직은 자식 클래스에서 수행
}

void Creature::KnockBack(CreatureRef attacker)
{
	// 플레이어가 몬스터를 바라보는 방향
	auto dir = attacker->GetLookAtDir(shared_from_this()->GetCellPos());
	Vec2Int backPos = {};

	switch (dir)
	{
	case DIR_UP:
		backPos = GetCellPos() + Vec2Int{ 0, -1 };
		break;

	case DIR_DOWN:
		backPos = GetCellPos() + Vec2Int{ 0, 1 };
		break;

	case DIR_LEFT:
		backPos = GetCellPos() + Vec2Int{ -1, 0 };
		break;

	case DIR_RIGHT:
		backPos = GetCellPos() + Vec2Int{ 1, 0 };
		break;

	default:
		return;
	}

	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(backPos))
		SetCellPos(backPos);
}

wstring Creature::GetName()
{
	return GET_SINGLE(ItemManager)->StringToWString(info.name());
}
