#include "pch.h"
#include "Item.h"
#include "Player.h"
#include "Chat.h"
#include "Quest.h"
#include "ItemManager.h"
#include "GameRoom.h"

Item::Item()
{
	info.set_dir(Protocol::DIR_TYPE_DOWN);
	info.set_objecttype(Protocol::OBJECT_TYPE_ITEM);
	_tickStart = GetTickCount64();
}

Item::~Item()
{
}

void Item::BeginPlay()
{
	// 자동 소멸 시간(ms)
	uint64 removeTime = itemInfo.removetime() * 1000;
	_tickStart = GetTickCount64() + removeTime;
}

void Item::Tick()
{
	_tickNow = GetTickCount64();

	// 드랍된 아이템의 주인과 접촉시 제거 표시
	if (IsTouched(_ownerId))
		_get = true;

	if (_get)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GChat->AddText(::format(L"player{0}가 위치 ({1}, {2})에서 ItemID {3}를 습득.", _ownerId, info.posx(), info.posy(), itemID));
	}

	// 10초 후 제거?
	if (_tickNow >= _tickStart)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GRoom->RemoveObject(info.objectid());

		GChat->AddText(::format(L"player{0} 소유,  위치 ({1}, {2})의 ItemID {3} 자동 소멸.", _ownerId, info.posx(), info.posy(), itemID));
	}
}

bool Item::IsTouched(uint64 playerId)
{
	CreatureRef creature = GRoom->GetCreatureAt({ info.posx(),info.posy() });
	if (creature)
		return creature->info.objectid() == playerId;

	return false;
}

void Item::TickIdle()
{
}