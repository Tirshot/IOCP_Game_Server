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
		GET_SINGLE(Quest)->ItemQuestCheck(_ownerId, itemID);

		GChat->AddText(L"player " + to_wstring(_ownerId) + L"�� ItemID" + to_wstring(itemID) + L" �� ����.");
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