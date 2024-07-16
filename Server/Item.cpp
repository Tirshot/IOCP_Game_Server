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
	// �ڵ� �Ҹ� �ð�(ms)
	uint64 removeTime = itemInfo.removetime() * 1000;
	_tickStart = GetTickCount64() + removeTime;
}

void Item::Tick()
{
	_tickNow = GetTickCount64();

	// ����� �������� ���ΰ� ���˽� ���� ǥ��
	if (IsTouched(_ownerId))
		_get = true;

	if (_get)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GChat->AddText(::format(L"player{0}�� ��ġ ({1}, {2})���� ItemID {3}�� ����.", _ownerId, info.posx(), info.posy(), itemID));
	}

	// 10�� �� ����?
	if (_tickNow >= _tickStart)
	{
		int itemID = itemInfo.itemid();
		wstring itemIDwstr = to_wstring(itemInfo.itemid());

		GRoom->RemoveObject(info.objectid());

		GChat->AddText(::format(L"player{0} ����,  ��ġ ({1}, {2})�� ItemID {3} �ڵ� �Ҹ�.", _ownerId, info.posx(), info.posy(), itemID));
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