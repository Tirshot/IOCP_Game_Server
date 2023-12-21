#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "GameRoom.h"

atomic<uint64> GameObject::s_idGenerator = 1;

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Update()
{

}

void GameObject::BroadcastMove()
{
	if (room)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(info);
		room->Broadcast(sendBuffer);
	}
}

PlayerRef GameObject::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	player->info.set_objectid(s_idGenerator++);
	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);

	return player;
}

MonsterRef GameObject::CreateMonster()
{
	MonsterRef monster = make_shared<Monster>();
	monster->info.set_objectid(s_idGenerator++);
	monster->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);

	return monster;
}

void GameObject::SetState(ObjectState state, bool broadcast)
{
	if (info.state() == state)
		return;

	info.set_state(state);

	if (broadcast)
		BroadcastMove();
}

void GameObject::SetDir(Dir dir, bool broadcast)
{
	info.set_dir(dir);

	if (broadcast)
		BroadcastMove();
}

bool GameObject::CanGo(Vec2Int cellPos)
{
	if (room == nullptr)
		return false;

	// 몬스터와 충돌

	return room->CanGo(cellPos);
}

Dir GameObject::GetLookAtDir(Vec2Int cellPos)
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

void GameObject::SetCellPos(Vec2Int cellPos, bool broadcast)
{
	info.set_posx(cellPos.x);
	info.set_posy(cellPos.y);

	if (broadcast)
		BroadcastMove();
}

Vec2Int GameObject::GetFrontCellPos()
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

void GameObject::OnDamaged(GameObjectRef attacker)
{
	if (attacker == nullptr)
		return;

	// disable PvP : 동족은 공격 불가
	if (this->info.objecttype() == attacker->info.objecttype())
		return;

	int32 damage = attacker->info.attack() - this->info.defence();
	attacker->info.set_damage(damage);

	if (damage <= 0)
		return;

	// hp는 항상 양수
	this->info.set_hp(max(0, this->info.hp() - damage));

	// 입힌 데미지를 브로드캐스트
	BroadcastMove();

	if (this->info.hp() == 0)
	{
		if (room)
		{
			room->RemoveObject(this->GetObjectID());
		}
	}
}
