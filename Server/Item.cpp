#include "pch.h"
#include "Item.h"
#include "Player.h"
#include "GameRoom.h"
#include "Chat.h"
#include "Quest.h"
#include "GameRoom.h"

Item::Item()
{
	info.set_dir(Protocol::DIR_TYPE_DOWN);
}

Item::~Item()
{
}

void Item::BeginPlay()
{

}

void Item::Tick()
{
	// 드랍된 아이템의 주인과 접촉시 제거 표시
	if (IsTouched(_ownerId))
		_get = true;

	if (_get)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GChat->AddText(::format(L"player{0}가 위치 ({1}, {2})에서 ItemID {3}를 습득.", _ownerId, info.posx(), info.posy(), itemID));
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