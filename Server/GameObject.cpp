#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Sign.h"
#include "Item.h"
#include "GameRoom.h"
#include "Arrow.h"
#include "Inventory.h"
#include "Trigger.h"
#include "Snake.h"
#include "Moblin.h"
#include "Quest1Trigger.h"

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

MonsterRef GameObject::CreateMonster(Protocol::MONSTER_TYPE monsterType)
{
	switch (monsterType)
	{
	case Protocol::MONSTER_TYPE_SNAKE:
	{
		SnakeRef snake = make_shared<Snake>();
		snake->info.set_objectid(s_idGenerator++);
		snake->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
		return snake;
	}

	case Protocol::MONSTER_TYPE_MOBLIN:
	{
		MoblinRef moblin = make_shared<Moblin>();
		moblin->info.set_objectid(s_idGenerator++);
		moblin->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
		return moblin;
	}

	default:
		return nullptr;
	}
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
	sign->info.set_objecttype(Protocol::OBJECT_TYPE_NPC);
	sign->info.set_npctype(Protocol::NPC_TYPE_SIGN);

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

InventoryRef GameObject::CreateInventory(PlayerRef player)
{
	InventoryRef inventory = make_shared<Inventory>();
	inventory->Init();
	inventory->SetOwner(player->GetObjectID());
	inventory->info.set_objecttype(Protocol::OBJECT_TYPE_INVENTORY);
	inventory->SetObjectID(player->GetObjectID());
	return inventory;
}

TriggerRef GameObject::CreateTrigger(uint64 questId)
{
	TriggerRef trigger;
	switch (questId)
	{
	case 1:
		trigger = make_shared<Quest1Trigger>();
		break;

	default:
		trigger = make_shared<Trigger>();
		break;
	}

	trigger->info.set_objectid(s_idGenerator++);
	trigger->info.set_objecttype(Protocol::OBJECT_TYPE_TRIGGER);
	trigger->SetQuestID(questId);
	return trigger;

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
	if (this->info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
		return room->MonsterCanGo(cellPos);

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

