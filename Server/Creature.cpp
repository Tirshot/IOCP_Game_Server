#include "pch.h"
#include "Creature.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Chat.h"
#include "Player.h"
#include "Item.h"

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

void Creature::OnDamaged(CreatureRef attacker)
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

	//// hp는 항상 양수
 //	info.set_hp(max(0, info.hp() - damage));

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

		if (info.hp() == 0)
		{
			// 플레이어만 막타를 쳤을 때 아이템이 드랍
			if (attacker->GetType() == Protocol::OBJECT_TYPE_PLAYER)
			{	// 아이템 드랍, 1 ~ 99
				auto randValueItem = (rand() % 99) + 1;

				if (randValueItem <= 10)
				{
					// 10%, HP 전체 회복
					if (room)
					{
						ItemRef item1 = GameObject::CreateItem();
						item1->info.set_posx(shared_from_this()->info.posx());
						item1->info.set_posy(shared_from_this()->info.posy());
						item1->info.set_defence(9999);
						item1->info.set_name("FullHeartItem");
						item1->info.set_itemtype(Protocol::ITEM_TYPE_FULLHEART);
						item1->SetOwner(attacker->GetObjectID());
						room->AddObject(item1);
						{
							Protocol::S_AddObject pkt;

							Protocol::ObjectInfo* info = pkt.add_objects();
							*info = item1->info;

							SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
							PlayerRef player = static_pointer_cast<Player>(room->FindObject(attacker->GetObjectID()));

							if (player)
								player->session->Send(sendBuffer);

							GChat->AddText(format(L"소유자 Player{0}, [{1}, {2}] 위치에 FullHeart 아이템 드랍.", attacker->info.objectid(), info->posx(), info->posy()));
						}
					}
				}
				else if (randValueItem > 10 && randValueItem <= 50)
				{
					// 40%, HP ++
					if (room)
					{
						ItemRef item1 = GameObject::CreateItem();
						item1->info.set_posx(shared_from_this()->info.posx());
						item1->info.set_posy(shared_from_this()->info.posy());
						item1->info.set_defence(9999);
						item1->info.set_name("HeartItem");
						item1->info.set_itemtype(Protocol::ITEM_TYPE_HEART);
						item1->SetOwner(attacker->GetObjectID());
						room->AddObject(item1);
						{
							Protocol::S_AddObject pkt;

							Protocol::ObjectInfo* info = pkt.add_objects();
							*info = item1->info;

							SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
							PlayerRef player = static_pointer_cast<Player>(room->FindObject(attacker->GetObjectID()));

							if (player)
								player->session->Send(sendBuffer);

							GChat->AddText(format(L"소유자 Player{0}, [{1}, {2}] 위치에 Heart 아이템 드랍.", attacker->info.objectid(), info->posx(), info->posy()));
						}
					}
				}
				else if (randValueItem > 50 && randValueItem <= 55)
				{
					// 5%, maxHP ++
					if (room)
					{
						ItemRef item1 = GameObject::CreateItem();
						item1->info.set_posx(shared_from_this()->info.posx());
						item1->info.set_posy(shared_from_this()->info.posy());
						item1->info.set_defence(9999);
						item1->info.set_name("MaxHeartItem");
						item1->info.set_itemtype(Protocol::ITEM_TYPE_MAXHEART);
						item1->SetOwner(attacker->GetObjectID());
						room->AddObject(item1);
						{
							Protocol::S_AddObject pkt;

							Protocol::ObjectInfo* info = pkt.add_objects();
							*info = item1->info;

							SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
							PlayerRef player = static_pointer_cast<Player>(room->FindObject(attacker->GetObjectID()));

							if (player)
								player->session->Send(sendBuffer);

							GChat->AddText(format(L"소유자 Player{0}, [{1}, {2}] 위치에 MaxHeart 아이템 드랍.", attacker->info.objectid(), info->posx(), info->posy()));
						}
					}
				}
				// 골드 드랍, 1~10
				auto randValueGold = (rand() % 10) + 1;
				attacker->info.set_gold(attacker->info.gold() + randValueGold);
				GChat->AddText(attacker->GetObjectID(), format(L"{0} 골드 획득.", randValueGold));
			}
			if (room)
			{
				room->RemoveObject(GetObjectID());
			
				// 채팅 출력
				GChat->AddText(format(L"{0} {1}이(가) {2} {3}에 의해 쓰러짐",
					objectType,
					GetObjectID(),
					attackerType,
					attacker->GetObjectID()));
			}
		}
	}
}

void Creature::KnockBack()
{
	// 캐릭터가 몬스터를 때릴때 몬스터만 넉백됨
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
