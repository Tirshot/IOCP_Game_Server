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

	// disable PvP : ������ ���� �Ұ�
	if (info.objecttype() == attacker->info.objecttype())
		return;

	// NPC ���� �Ұ�
	if (info.objecttype() == Protocol::OBJECT_TYPE_NPC || info.objecttype() == Protocol::OBJECT_TYPE_NPC_SIGN)
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	//// hp�� �׻� ���
 //	info.set_hp(max(0, info.hp() - damage));

	// ä�� ���
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

		GChat->AddText(format(L"{0} {1}��(��) {2} {3}���� {4}�� ���ظ� ����",
			attackerType,
			attacker->GetObjectID(),
			objectType,
			GetObjectID(),
			damage));

		if (info.hp() == 0)
		{
			// �÷��̾ ��Ÿ�� ���� �� �������� ���
			if (attacker->GetType() == Protocol::OBJECT_TYPE_PLAYER)
			{	// ������ ���, 1 ~ 99
				auto randValueItem = (rand() % 99) + 1;

				if (randValueItem <= 10)
				{
					// 10%, HP ��ü ȸ��
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

							GChat->AddText(format(L"������ Player{0}, [{1}, {2}] ��ġ�� FullHeart ������ ���.", attacker->info.objectid(), info->posx(), info->posy()));
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

							GChat->AddText(format(L"������ Player{0}, [{1}, {2}] ��ġ�� Heart ������ ���.", attacker->info.objectid(), info->posx(), info->posy()));
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

							GChat->AddText(format(L"������ Player{0}, [{1}, {2}] ��ġ�� MaxHeart ������ ���.", attacker->info.objectid(), info->posx(), info->posy()));
						}
					}
				}
				// ��� ���, 1~10
				auto randValueGold = (rand() % 10) + 1;
				attacker->info.set_gold(attacker->info.gold() + randValueGold);
				GChat->AddText(attacker->GetObjectID(), format(L"{0} ��� ȹ��.", randValueGold));
			}
			if (room)
			{
				room->RemoveObject(GetObjectID());
			
				// ä�� ���
				GChat->AddText(format(L"{0} {1}��(��) {2} {3}�� ���� ������",
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
	// ĳ���Ͱ� ���͸� ������ ���͸� �˹��
	if (CanGo(-GetFrontCellPos()))
		SetCellPos(-GetFrontCellPos(), true);
}
