#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Sign.h"
#include "Item.h"
#include "GameRoom.h"
#include "Arrow.h"

atomic<uint64> GameObject::s_idGenerator = 1;

GameObject::GameObject()
{

}

GameObject::~GameObject()
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
	player->info.set_name("Player");
	return player;
}

MonsterRef GameObject::CreateMonster()
{
	MonsterRef monster = make_shared<Monster>();
	monster->info.set_objectid(s_idGenerator++);
	monster->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
	 
	return monster;
}

NPCRef GameObject::CreateNPC()
{
	NPCRef npc = make_shared<NPC>();
	npc->info.set_objectid(s_idGenerator++);
	npc->info.set_objecttype(Protocol::OBJECT_TYPE_NPC);
	 
	return npc;
}

SignRef GameObject::CreateSign()
{
	SignRef sign = make_shared<Sign>();
	sign->info.set_objectid(s_idGenerator++);
	sign->info.set_objecttype(Protocol::OBJECT_TYPE_NPC_SIGN);

	return sign;
}

ItemRef GameObject::CreateItem()
{
	ItemRef item = make_shared<Item>();
	item->info.set_objectid(s_idGenerator++);
	item->info.set_objecttype(Protocol::OBJECT_TYPE_ITEM);

	return item;
}

ArrowRef GameObject::CreateArrow()
{
	ArrowRef arrow = make_shared<Arrow>();
	arrow->info.set_objectid(s_idGenerator++);
	arrow->info.set_objecttype(Protocol::OBJECT_TYPE_PROJECTILE);

	return arrow->weak_from_this().lock();
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

