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
	// ����� �������� ���ΰ� ���˽� ���� ǥ��
	if (IsTouched(_ownerId))
		_get = true;

	if (_get)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GChat->AddText(::format(L"player{0}�� ��ġ ({1}, {2})���� ItemID {3}�� ����.", _ownerId, info.posx(), info.posy(), itemID));
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