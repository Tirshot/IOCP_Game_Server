#include "pch.h"
#include "Creature.h"
#include "Player.h"
#include "Monster.h"
#include "GameRoom.h"

atomic<uint64> Creature::s_idGenerator = 1;

Creature::Creature()
{

}

Creature::~Creature()
{

}


void Creature::Update()
{

}

void Creature::BroadcastMove()
{
	if (room)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(info);
		room->Broadcast(sendBuffer);
	}
}

PlayerRef Creature::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	player->info.set_objectid(s_idGenerator++);
	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);

	return player;
}

MonsterRef Creature::CreateMonster()
{
	MonsterRef monster = make_shared<Monster>();
	monster->info.set_objectid(s_idGenerator++);
	monster->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);

	return monster;
}

void Creature::SetState(ObjectState state, bool broadcast)
{
	if (info.state() == state)
		return;

	info.set_state(state);

	if (broadcast)
		BroadcastMove();
}

void Creature::SetDir(Dir dir, bool broadcast)
{
	info.set_dir(dir);

	if (broadcast)
		BroadcastMove();
}

bool Creature::CanGo(Vec2Int cellPos)
{
	if (room == nullptr)
		return false;

	// 몬스터와 충돌

	return room->CanGo(cellPos);
}

Dir Creature::GetLookAtDir(Vec2Int cellPos)
{
	Vec2Int dir = cellPos - GetCellPos();
	if (dir.x > 0)
		return DIR_RIGHT;
	else if (dir.x < 0)
		return DIR_LEFT;
	else if (dir.y > 0)
		return DIR_DOWN;
	else
		return DIR_UP;
}

void Creature::SetCellPos(Vec2Int cellPos, bool broadcast)
{
	info.set_posx(cellPos.x);
	info.set_posy(cellPos.y);

	if (broadcast)
		BroadcastMove();
}

Vec2Int Creature::GetFrontCellPos()
{
	Vec2Int pos = GetCellPos();

	switch (info.dir())
	{
	case DIR_DOWN:
		return pos + Vec2Int{ 0,1 };
	case DIR_LEFT:
		return pos + Vec2Int{ -1,0 };
	case DIR_RIGHT:
		return pos + Vec2Int{ 1,0 };
	case DIR_UP:
		return pos + Vec2Int{ 0,-1 };
	}

	return pos;
}

void Creature::OnDamaged(CreatureRef attacker)
{
	if (attacker == nullptr)
		return;

	// disable PvP : 동족은 공격 불가
	if (info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - this->info.defence();
	attacker->info.set_damage(damage);

	if (damage <= 0)
		return;

	// hp는 항상 양수
	info.set_hp(max(0, info.hp() - damage));

	// 입힌 데미지를 브로드캐스트
	BroadcastMove();

	if (info.hp() == 0)
	{
		if (room)
		{
			room->RemoveObject(GetObjectID());
		}
	}
}
