#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "Chat.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Arrow.h"
#include "Item.h"
#include "Inventory.h"

void ServerPacketHandler::HandlePacket(GameSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	// 클라이언트 -> 서버
	switch (header.id)
	{
	case C_Move:
		Handle_C_Move(session, buffer, len);
		break;

	case C_Hit:
		Handle_C_Hit(session, buffer, len);
		break;

	case C_Fire:
		Handle_C_Fire(session, buffer, len);
		break;

	case C_SendMessage:
		Handle_C_SendMessage(session, buffer, len);
		break;

	case C_Revive:
		Handle_C_Revive(session, buffer, len);
		break;

	case C_Quest:
		Handle_C_Quest(session, buffer, len);
		break;

	case C_QuestFinish:
		Handle_C_QuestFinish(session, buffer, len);
		break;

	case C_QuestList:
		Handle_C_QuestList(session, buffer, len);
		break;

	case C_Heal:
		Handle_C_Heal(session, buffer, len);
		break;

	case C_AddItem:
		Handle_C_AddItem(session, buffer, len);
		break;

	case C_EquipItem:
		Handle_C_EquipItem(session, buffer, len);
		break;

	case C_SyncInventory:
		Handle_C_SyncInventory(session, buffer, len);
		break;

	case C_KillPlayer:
		Handle_C_KillPlayer(session, buffer, len);
		break;

	default:
		break;
	}
}

void ServerPacketHandler::Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	// session->gameRoom
	GameRoomRef room = session->gameRoom.lock();
	if (room)
		room->Handle_C_Move(pkt);
}

void ServerPacketHandler::Handle_C_Hit(GameSessionRef session, BYTE* buffer, int32 len)
{
	// DEBUG!!!
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Hit pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	int attackerID = pkt.attackerid();
	int objectID = pkt.objectid();

	GameRoomRef room = session->gameRoom.lock();
	if (room)
	{
		MonsterRef monster = dynamic_pointer_cast<Monster>(room->FindObject(objectID));
		PlayerRef player = dynamic_pointer_cast<Player>(room->FindObject(attackerID));

		if (monster && player)
			monster->OnDamaged(player, true);

	}
}

void ServerPacketHandler::Handle_C_Fire(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Fire pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameRoomRef room = session->gameRoom.lock();
	if (room)
	{
		auto id = pkt.ownerid();
		auto object = room->FindObject(id);
		GChat->AddText(format(L"서버에서 player{0}가 화살 생성", pkt.ownerid()));
		if (object)
		{
			auto player = dynamic_pointer_cast<Player>(object);
			if (player)
			{
				player->MakeArrow();
			}
		}
	}

}

void ServerPacketHandler::Handle_C_SendMessage(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_SendMessage pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	Protocol::Text texts = *pkt.mutable_texts();

	uint64 objectId = texts.objectid();
	time_t now = texts.time();
	string str = texts.str();
	bool broadcast = texts.broadcast();

	GameRoomRef room = session->gameRoom.lock();

	if (room)
	{
		wstring wstr = GChat->StringToWStr(str);
		GChat->AddText(::format(L"[{0}] Player {1} :{2}", now, objectId, wstr));

		if (broadcast)
		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendMessage(objectId, now, str);
			room->Broadcast(sendBuffer);
		}
	}
}

void ServerPacketHandler::Handle_C_Revive(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Revive pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	uint64 id = pkt.info().objectid();
	Protocol::ObjectInfo info = *pkt.mutable_info();

	GameRoomRef room = session->gameRoom.lock();

	if (room)
	{
		room->LeaveRoom(session);

		// 플레이어 초기화
		PlayerRef player = GameObject::CreatePlayer();
		player->info = info;
		player->info.set_objectid(id);
		player->info.set_hp(info.maxhp());
		player->info.set_mp(info.maxmp());
		player->info.set_posx(5);
		player->info.set_posy(5);
		player->info.set_weapontype(Protocol::WEAPON_TYPE_SWORD);

		// 클라이언트 서로의 존재를 연결
		session->gameRoom = room;
		session->player = player;
		player->session = session;
		player->info.set_dir(Protocol::DIR_TYPE_DOWN);

		// 모든 오브젝트 정보 전송
		{
			for (auto& item : room->GetPlayers())
			{
				Protocol::S_AddObject pkt;

				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;

				{	// 오브젝트 정보 추가
					SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt, true);
					session->Send(sendBuffer);
				}
			}

			for (auto& item : room->GetMonsters())
			{
				Protocol::S_AddObject pkt;

				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;

				{	// 오브젝트 정보 추가
					SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
					session->Send(sendBuffer);
				}
			}

			for (auto& item : room->GetNPCs())
			{
				Protocol::S_AddObject pkt;

				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;

				{	// 오브젝트 정보 추가
					SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
					session->Send(sendBuffer);
				}
			}
		}
		//{	// Scene 리셋
		//	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Reset();
		//	session->Send(sendBuffer);
		//}
		{	// 입장한 클라이언트에게 정보 전송
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->info, true);
			session->Send(sendBuffer);
		}

		{
			// 드랍 아이템 재생성
			Protocol::S_ItemDrop pkt;
			for (auto& item : room->GetItems())
			{
				Protocol::ItemInfo* info = pkt.mutable_iteminfo();
				*info = item.second->itemInfo;

				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_ItemDrop(*info);
				session->Send(sendBuffer);
			}
		}
		{
			// 퀘스트 정보 재전송
			auto questsStates = room->GetQuestsStatesByID(id);

			Protocol::S_QuestState pkt;
			for (auto& state : questsStates)
			{
				int questID = state.first;
				Protocol::QUEST_STATE questState = state.second.state;
				int progress = state.second.progress;

				{	// 퀘스트 진행도 패킷 전송
					Protocol::QuestInfo* info = pkt.mutable_questinfo();
					info->set_questid(questID);
					info->set_queststate(questState);
					info->set_process(progress);

					SendBufferRef sendBuffer = ServerPacketHandler::Make_S_QuestState(*info);
					session->Send(sendBuffer);
				}
			}
		}
		room->RemoveTemp(id);
		room->AddObject(player);
		GChat->AddText(format(L"Player {0}이 부활함.", id));
	}
}

void ServerPacketHandler::Handle_C_Quest(GameSessionRef session, BYTE* buffer, int32 len)
{	// 퀘스트 수락
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Quest pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
	//
	uint64 objectid = pkt.objectid();
	uint64 questid = pkt.questid();
	GameRoomRef room = session->gameRoom.lock();

	if (room)
	{
		room->SetQuestStates(objectid, questid, Protocol::QUEST_STATE_ACCEPT);
	}
}

void ServerPacketHandler::Handle_C_QuestFinish(GameSessionRef session, BYTE* buffer, int32 len)
{   // 퀘스트 완료 처리
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Quest pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
	//
	uint64 objectid = pkt.objectid();
	uint64 questid = pkt.questid();
	GameRoomRef room = session->gameRoom.lock();

	if (room)
	{
		room->SetQuestStates(objectid, questid, Protocol::QUEST_STATE_FINISHED);
	}
}

void ServerPacketHandler::Handle_C_QuestList(GameSessionRef session, BYTE* buffer, int32 len)
{	// 퀘스트 리스트 요청 처리
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_QuestList pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
	//
	GameRoomRef room = session->gameRoom.lock();

	if (room)
	{
		auto quests = room->GetQuests();
		auto player = session->player.lock();

		if (player)
		{
			uint64 objectId = player->info.objectid();

			for (auto& quest : quests)
			{
				int questid = quest.first;
				Protocol::QuestInfo job = quest.second;
				{
					SendBufferRef sendBuffer = Make_S_QuestList(objectId, job);
					session->Send(sendBuffer);
				}
			}
		}
	}
}

void ServerPacketHandler::Handle_C_Heal(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_QuestList pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
	//
	GameRoomRef room = session->gameRoom.lock();
	
	PlayerRef healedPlayer = session->player.lock();
	healedPlayer->info.set_hp(healedPlayer->info.hp() + 1);
}

void ServerPacketHandler::Handle_C_AddItem(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;

	uint16 size = header->size;

	Protocol::C_AddItem pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameRoomRef room = session->gameRoom.lock();
	PlayerRef myPlayer = session->player.lock();

	auto* item = pkt.mutable_iteminfo();
	int itemId = item->itemid();
	int itemCounts = item->itemcount();
	Protocol::ITEM_TYPE itemType = item->itemtype();
	int index = pkt.index();

	// 가져온 패킷의 정보를 이용, 플레이어 인벤토리 관리
	if (room && myPlayer)
	{
		room->AddItemToPlayer(myPlayer->GetObjectID(), itemId, itemCounts, itemType, index);
	}
}

void ServerPacketHandler::Handle_C_EquipItem(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;

	uint16 size = header->size;

	Protocol::C_EquipItem pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameRoomRef room = session->gameRoom.lock();
	PlayerRef myPlayer = session->player.lock();

	int itemId = pkt.itemid();
	bool equip = pkt.equip();

	// 가져온 패킷의 정보를 이용, 플레이어 인벤토리 관리
	if (room && myPlayer)
	{
		room->EquipItemToPlayer(myPlayer->GetObjectID(), itemId, equip);
	}
}

void ServerPacketHandler::Handle_C_SyncInventory(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;

	uint16 size = header->size;

	Protocol::C_SyncInventory pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameRoomRef room = session->gameRoom.lock();

	int objectID = pkt.objectid();

	auto inventory = room->GetInventory(objectID);

	inventory->SyncToClient(objectID);
}

void ServerPacketHandler::Handle_C_KillPlayer(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;

	uint16 size = header->size;

	Protocol::C_KillPlayer pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameRoomRef room = session->gameRoom.lock();

	int objectID = pkt.objectid();
	
	auto player = dynamic_pointer_cast<Player>(room->FindObject(objectID)); 
	if (player)
	{
		player->info.set_hp(0);
		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(player->info);
			player->session->Send(sendBuffer);
		}
	}
}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs)
{
	Protocol::S_TEST pkt;
	pkt.set_id(10);
	pkt.set_hp(100);
	pkt.set_attack(10);

	{
		Protocol::BuffData* data = pkt.add_buffs();
		data->set_buffid(100);
		data->set_remaintime(1.2f);
		{
			data->add_victims(10);
		}
	}
	{
		Protocol::BuffData* data = pkt.add_buffs();
		data->set_buffid(200);
		data->set_remaintime(2.2f);
		{
			data->add_victims(20);
		}
	}

	return MakeSendBuffer(pkt, S_TEST);
}

SendBufferRef ServerPacketHandler::Make_S_Hit(uint64 objectId, uint64 attackerId, int32 damage)
{
	Protocol::S_Hit pkt;

	pkt.set_objectid(objectId);
	pkt.set_attackerid(attackerId);
	pkt.set_damage(damage);

	return MakeSendBuffer(pkt, S_Hit);
}

SendBufferRef ServerPacketHandler::Make_S_Reset()
{
	Protocol::S_Reset pkt;

	return MakeSendBuffer(pkt, S_Reset);
}

SendBufferRef ServerPacketHandler::Make_S_EnterGame()
{
	Protocol::S_EnterGame pkt;

	pkt.set_success(true);
	pkt.set_accountid(0); // DB에서 가져옴

	// Searialize
	return MakeSendBuffer(pkt, S_EnterGame);
}

SendBufferRef ServerPacketHandler::Make_S_MyPlayer(const Protocol::ObjectInfo& info, bool revive)
{
	Protocol::S_MyPlayer pkt;

	// Struct 내의 Struct -> mutable_info, 포인터 반환
	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;
	pkt.set_revive(revive);

	return MakeSendBuffer(pkt, S_MyPlayer);
}

SendBufferRef ServerPacketHandler::Make_S_AddObject(Protocol::S_AddObject& pkt, bool revive)
{
	pkt.set_revive(revive);
	return MakeSendBuffer(pkt, S_AddObject);
}

SendBufferRef ServerPacketHandler::Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt)
{
	return MakeSendBuffer(pkt, S_RemoveObject);
}

SendBufferRef ServerPacketHandler::Make_S_Move(const Protocol::ObjectInfo& info)
{
	Protocol::S_Move pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	return MakeSendBuffer(pkt, S_Move);
}

SendBufferRef ServerPacketHandler::Make_S_SendMessage(uint64 objectId, uint64 time, string str)
{
	Protocol::S_SendMessage pkt;
	auto* texts = pkt.mutable_texts();
	texts->set_objectid(objectId);
	texts->set_time(time);
	texts->set_str(str);

	return MakeSendBuffer(pkt, S_SendMessage);
}

SendBufferRef ServerPacketHandler::Make_S_Teleport(uint64 objectId, int32 cellPosX, int32 cellPosY)
{
	// 패킷 생성
	Protocol::S_Teleport pkt;

	pkt.set_objectid(objectId);
	pkt.set_posx(cellPosX);
	pkt.set_posy(cellPosY);

	return MakeSendBuffer(pkt, S_Teleport);
}

SendBufferRef ServerPacketHandler::Make_S_Gold(uint64 objectId, int32 gold)
{
	// 패킷 생성
	Protocol::S_Gold pkt;

	pkt.set_objectid(objectId);
	pkt.set_gold(gold);

	return MakeSendBuffer(pkt, S_Gold);
}

SendBufferRef ServerPacketHandler::Make_S_MPRecover(uint64 objectId, int mp)
{
	// 패킷 생성
	Protocol::S_MPRecover pkt;

	pkt.set_objectid(objectId);
	pkt.set_mp(mp);

	return MakeSendBuffer(pkt, S_MPRecover);
}


SendBufferRef ServerPacketHandler::Make_S_QuestProcess(uint64 objectid, uint64 questid, uint64 process)
{
	Protocol::S_QuestProcess pkt;

	Protocol::QuestInfo* questInfo = pkt.mutable_questinfo();
	questInfo->set_objectid(objectid);
	questInfo->set_questid(questid);
	questInfo->set_process(process);
	questInfo->set_queststate(Protocol::QUEST_STATE_ACCEPT);

	int targetNums = GRoom->GetQuest(questid).targetnums();

	GChat->AddText(::format(L"player {0}, quest {1} [{2} / {3}] 진행.", objectid, questid, process, targetNums));

	return MakeSendBuffer(pkt, S_QuestProcess);
}

SendBufferRef ServerPacketHandler::Make_S_QuestComplete(uint64 objectid, uint64 questid, uint64 process)
{
	Protocol::S_QuestComplete pkt;

	Protocol::QuestInfo* questInfo = pkt.mutable_questinfo();
	questInfo->set_objectid(objectid);
	questInfo->set_questid(questid);
	questInfo->set_process(process);
	questInfo->set_queststate(Protocol::QUEST_STATE_COMPLETED);
	GChat->AddText(::format(L"player {0}, quest {1} 완료.", objectid, questid));

	return MakeSendBuffer(pkt, S_QuestComplete);
}

SendBufferRef ServerPacketHandler::Make_S_QuestList(uint64 objectid, const Protocol::QuestInfo& info)
{
	Protocol::S_QuestList pkt;

	Protocol::QuestInfo* questInfo = pkt.mutable_questinfo();
	*questInfo = info;
	questInfo->set_objectid(objectid);

	return MakeSendBuffer(pkt, S_QuestList);
}

SendBufferRef ServerPacketHandler::Make_S_QuestState(const Protocol::QuestInfo& info)
{
	Protocol::S_QuestState pkt;

	Protocol::QuestInfo* questInfo = pkt.mutable_questinfo();
	*questInfo = info;

	return MakeSendBuffer(pkt, S_QuestState);
}

SendBufferRef ServerPacketHandler::Make_S_ItemDrop(const Protocol::ItemInfo& info)
{
	Protocol::S_ItemDrop pkt;

	Protocol::ItemInfo* itemInfo = pkt.mutable_iteminfo();
	*itemInfo = info;

	return MakeSendBuffer(pkt, S_ItemDrop);
}

SendBufferRef ServerPacketHandler::Make_S_AddItem(uint64 objectID, int itemID, int itemCounts)
{
	Protocol::S_AddItem pkt;

	pkt.set_objectid(objectID);
	pkt.set_itemid(itemID);
	pkt.set_itemcounts(itemCounts);

	return MakeSendBuffer(pkt, S_AddItem);
}

SendBufferRef ServerPacketHandler::Make_S_Fire(const Protocol::ObjectInfo& info, uint64 objectID)
{
	Protocol::S_Fire pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;
	pkt.set_ownerid(objectID);

	return MakeSendBuffer(pkt, S_Fire);
}
