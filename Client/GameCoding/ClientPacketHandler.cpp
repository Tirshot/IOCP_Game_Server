#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "QuestManager.h"
#include "Game.h"
#include "Monster.h"
#include "HitEffect.h"
#include "TeleportEffect.h"
#include "Creature.h"
#include "Player.h"
#include "MyPlayer.h"
#include "Chat.h"
#include "Inventory.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "ItemManager.h"

void ClientPacketHandler::HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
		case S_EnterGame:
		Handle_S_EnterGame(session, buffer, len);
		break;

		case S_MyPlayer:
			Handle_S_MyPlayer(session, buffer, len);
			break;

		case S_AddObject:
			Handle_S_AddObject(session, buffer, len);
			break;

		case S_RemoveObject:
			Handle_S_RemoveObject(session, buffer, len);
			break;

		case S_Move:
			Handle_S_Move(session, buffer, len);
			break;

		case S_Hit:
			Handle_S_Hit(session, buffer, len);
			break;

		case S_Fire:
			Handle_S_Fire(session, buffer, len);
			break;

		case S_MPRecover:
			Handle_S_MPRecover(session, buffer, len);
			break;

		case S_SendMessage:
			Handle_S_SendMessage(session, buffer, len);
			break;

		case S_Teleport:
			Handle_S_Teleport(session, buffer, len);
			break;

		case S_Gold:
			Handle_S_Gold(session, buffer, len);
			break;

		case S_Quest:
			Handle_S_Quests(session, buffer, len);
			break;

		case S_QuestProcess:
			Handle_S_QuestProcess(session, buffer, len);
			break;

		case S_QuestComplete:
			Handle_S_QuestComplete(session, buffer, len);
			break;

		case S_QuestList:
			Handle_S_QuestList(session, buffer, len);
			break;

		case S_QuestState:
			Handle_S_QuestState(session, buffer, len);
			break;

		case S_Reset:
			Handle_S_Reset(session, buffer, len);
			break;

		case S_ItemDrop:
			Handle_S_ItemDrop(session, buffer, len);
			break;

		case S_AddItem:
			Handle_S_AddItem(session, buffer, len);
			break;
	}
}

void ClientPacketHandler::Handle_S_TEST(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_TEST pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	uint64 id = pkt.id();
	uint32 hp = pkt.hp();
	uint16 attack = pkt.attack();

	//cout << "ID: " << id << " HP : " << hp << " ATT : " << attack << endl;

	for (int32 i = 0; i < pkt.buffs_size(); i++)
	{
		const Protocol::BuffData& data = pkt.buffs(i);
		/*cout << "BuffInfo : " << data.buffid() << " " << data.remaintime() << endl;*/
	}
}

void ClientPacketHandler::Handle_S_Reset(ServerSessionRef session, BYTE* buffer, int32 len)
{
	// Scene 초기화
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);
}

void ClientPacketHandler::Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len)
{
	// 서버의 코드와 대칭적으로 작성
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_EnterGame pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	bool success = pkt.success();
	uint64 accountId = pkt.accountid();
}

void ClientPacketHandler::Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_MyPlayer pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	bool revive = pkt.revive();

	// 마이 플레이어 배치 및 정보 동기화
	const Protocol::ObjectInfo& info = pkt.info();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		shared_ptr<MyPlayer> myPlayer = scene->SpawnObject<MyPlayer>(Vec2Int{ info.posx(), info.posy() });
		myPlayer->info = info;
		myPlayer->SetDir(info.dir());
		myPlayer->SetState(info.state());
		GET_SINGLE(SceneManager)->SetMyPlayer(myPlayer);
		GET_SINGLE(QuestManager)->BeginPlay();

		if (revive)
		{
			// 인벤토리 초기화
			GET_SINGLE(ItemManager)->ResetInventory();

			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_SyncInventory(myPlayer->GetObjectID());
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
	}
}

void ClientPacketHandler::Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_AddObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		scene->Handle_S_AddObject(pkt);

}

void ClientPacketHandler::Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_RemoveObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		scene->Handle_S_RemoveObject(pkt);
}

void ClientPacketHandler::Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	
	uint16 size = header->size;

	Protocol::S_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::ObjectInfo& info = pkt.info();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		shared_ptr<GameObject> gameObject = scene->GetObjects(info.objectid());
		if (gameObject == nullptr)
			return;

		uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();

		if (myPlayerId == info.objectid())
		{
			//gameObject->info.set_arrows(info.arrows());
			gameObject->info.set_hp(info.hp());
			//gameObject->info.set_mp(info.mp());
			//gameObject->info.set_gold(info.gold());
		}
		else
		{
			gameObject->SetDir(info.dir());
			gameObject->SetState(info.state());
			gameObject->SetCellPos(Vec2Int{ info.posx(), info.posy() });
			gameObject->SetWeaponType(info.weapontype());
			gameObject->info.set_arrows(info.arrows());
			gameObject->info.set_hp(info.hp());
			gameObject->info.set_mp(info.mp());
			gameObject->info.set_gold(info.gold());
		}
	}
}

void ClientPacketHandler::Handle_S_Hit(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Hit pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	uint64 objectId = pkt.objectid();
	uint64 attackerId = pkt.attackerid();
	int32 damage = pkt.damage();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	shared_ptr<Creature> creature = dynamic_pointer_cast<Creature>(scene->GetObjects(objectId));

	if (creature == nullptr)
		return;

	if (creature->info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
	{
		GET_SINGLE(SoundManager)->Play(L"MonsterOnDamaged");
	}
	else if (creature->info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
	{
		scene->SpawnObject<HitEffect>(creature->GetCellPos());
		GET_SINGLE(SoundManager)->Play(L"PlayerOnDamaged");
	}

	creature->info.set_hp(clamp(creature->info.hp() - damage, 0, creature->info.maxhp()));
}

void ClientPacketHandler::Handle_S_Fire(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Fire pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::ObjectInfo& info = pkt.info();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	// 화살의 주인 플레이어를 가져옴
	if (scene)
	{
		auto player = scene->GetPlayerByID(pkt.ownerid());

		if (player)
		{
			player->MakeArrow();
		}
	}
}

void ClientPacketHandler::Handle_S_MPRecover(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_MPRecover pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	shared_ptr<MyPlayer> myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	if (myPlayer)
	{
		myPlayer->info.set_mp(pkt.mp());
	}

}

void ClientPacketHandler::Handle_S_SendMessage(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_SendMessage pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::Text& texts = pkt.texts();
	uint64 objectId = texts.objectid();
	string name = GET_SINGLE(SceneManager)->GetPlayerByID(objectId)->info.name();
	wstring wname = GET_SINGLE(ChatManager)->StringToWStr(name);

	time_t now = texts.time();
	wstring nowTime = GET_SINGLE(ChatManager)->ChangeTimeFormat(now);

	string str = texts.str();
	wstring wstr = GET_SINGLE(ChatManager)->StringToWStr(str);

	shared_ptr<Chat> chat = GET_SINGLE(ChatManager)->GetChat();

	chat->AddText(format(L"[{0}] {1} :{2}", nowTime, wname.c_str(), wstr));
}

void ClientPacketHandler::Handle_S_Teleport(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Teleport pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	uint64 objectId = pkt.objectid();
	shared_ptr<Player> player = GET_SINGLE(SceneManager)->GetPlayerByID(objectId);

	if (player)
	{
		shared_ptr<MyPlayer> myPlayer = static_pointer_cast<MyPlayer>(player);
		myPlayer->Teleport(Vec2Int {pkt.posx(), pkt.posy()});

		auto scene = GET_SINGLE(SceneManager)->GetDevScene();
		scene->SpawnObject<TeleportEffect>(Vec2Int{ pkt.posx(), pkt.posy() });
	}
}

void ClientPacketHandler::Handle_S_Gold(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Gold pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	uint64 objectId = pkt.objectid();
	int32 gold = pkt.gold();

	shared_ptr<MyPlayer> myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer)
	{
		myPlayer->info.set_gold(myPlayer->info.gold() + gold);
		GET_SINGLE(ChatManager)->AddMessage(format(L"{0} 골드 획득", gold));
	}
}

void ClientPacketHandler::Handle_S_Quests(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Quest pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	const Protocol::QuestInfo& questInfo = pkt.questinfo();

	if (scene)
	{
		scene->AddQuest(questInfo);
	}
}

void ClientPacketHandler::Handle_S_QuestProcess(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_QuestProcess pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	uint64 objectId = pkt.questinfo().objectid();
	uint64 questId = pkt.questinfo().questid();
	int process = pkt.questinfo().process();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	shared_ptr<MyPlayer> player = GET_SINGLE(SceneManager)->GetMyPlayer();
	player->SetQuestProgress(questId, process);

	{
		GET_SINGLE(ChatManager)->AddMessage(format(L"퀘스트 진행 중, {0} / {1} 완료", process, scene->GetQuest(questId).targetnums()));
	}
	if (player)
	{
		player->SetQuestState(questId, Protocol::QUEST_STATE_ACCEPT ,process);
	}
}

void ClientPacketHandler::Handle_S_QuestComplete(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_QuestComplete pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	uint64 objectId = pkt.questinfo().objectid();
	uint64 questId = pkt.questinfo().questid();
	int process = pkt.questinfo().process();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	shared_ptr<MyPlayer> player = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (player)
	{
		player->SetQuestState(questId, Protocol::QUEST_STATE_COMPLETED, process);
		GET_SINGLE(ChatManager)->AddMessage(L"QUEST COMPLETE!!");
		GET_SINGLE(ChatManager)->AddMessage(L"상인에게 돌아가서 보상을 받으세요.");
		GET_SINGLE(SoundManager)->Play(L"QuestComplete");
	}
}

void ClientPacketHandler::Handle_S_QuestList(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_QuestList pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	Protocol::QuestInfo info = pkt.questinfo();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		scene->SetQuests( info.questid(), info);
	}
}

void ClientPacketHandler::Handle_S_QuestState(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_QuestList pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	Protocol::QuestInfo info = pkt.questinfo();

	int questID = info.questid();
	Protocol::QUEST_STATE questState = info.queststate();
	int progress = info.process();

	shared_ptr<MyPlayer> myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer)
	{
		myPlayer->SetQuestState(questID, questState);
		myPlayer->SetQuestProgress(questID, progress);
	}
}

void ClientPacketHandler::Handle_S_ItemDrop(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_ItemDrop pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	Protocol::ItemInfo info = pkt.iteminfo();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		scene->SpawnItem(info);
	}
}

void ClientPacketHandler::Handle_S_AddItem(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_AddItem pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	int objectID = pkt.objectid();
	int itemID = pkt.itemid();
	int itemCounts = pkt.itemcounts();

	shared_ptr<MyPlayer> myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	shared_ptr<Inventory> inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (myPlayer && inventory)
	{
		GET_SINGLE(ItemManager)->SetItemToInventory(itemID, itemCounts);
	}

}

// 패킷 보내기
SendBufferRef ClientPacketHandler::Make_C_Move()
{
	// 패킷 생성
	Protocol::C_Move pkt;

	// MyPlayer를 가져옴
	shared_ptr<MyPlayer> myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	// 패킷의 info를 수정해 myPlayer의 정보를 패킷에 담음
	*pkt.mutable_info() = myPlayer->info;
	
	return MakeSendBuffer(pkt, C_Move);
}

SendBufferRef ClientPacketHandler::Make_C_Fire(uint64 objectId)
{
	// 패킷 생성
	Protocol::C_Fire pkt;

	pkt.set_ownerid(objectId);

	return MakeSendBuffer(pkt, C_Fire);
}

SendBufferRef ClientPacketHandler::Make_C_Hit(uint64 objectId, uint64 attackerId)
{
	// 패킷 생성
	Protocol::C_Hit pkt;

	pkt.set_attackerid(attackerId);
	pkt.set_objectid(objectId);

	return MakeSendBuffer(pkt, C_Hit);
}

SendBufferRef ClientPacketHandler::Make_C_SendMessage(uint64 objectId, time_t time, string str, bool broadcast)
{
	Protocol::C_SendMessage pkt;

	auto* texts = pkt.mutable_texts();
	texts->set_objectid(objectId);
	texts->set_time(time);
	texts->set_str(str);
	texts->set_broadcast(broadcast);

	return MakeSendBuffer(pkt, C_SendMessage);
}

SendBufferRef ClientPacketHandler::Make_C_Revive(Protocol::ObjectInfo& objectInfo)
{
	Protocol::C_Revive pkt;

	*pkt.mutable_info() = objectInfo;

	int objectId = objectInfo.objectid();

	GET_SINGLE(ChatManager)->SendMessageToServer(L"부활 요청.", false);

	return MakeSendBuffer(pkt, C_Revive);
}

SendBufferRef ClientPacketHandler::Make_C_Quest(uint64 objectId, uint64 questId)
{
	Protocol::C_Quest pkt;

	pkt.set_objectid(objectId);
	pkt.set_questid(questId);
	GET_SINGLE(ChatManager)->AddMessage(L"퀘스트를 수락했습니다.");
	GET_SINGLE(ChatManager)->SendMessageToServer(::format(L"Quest {0}을 수락함.", questId), false);

	return MakeSendBuffer(pkt, C_Quest);
}

SendBufferRef ClientPacketHandler::Make_C_QuestFinish(uint64 objectId, uint64 questId)
{
	Protocol::C_QuestFinish pkt;

	pkt.set_objectid(objectId);
	pkt.set_questid(questId);
	GET_SINGLE(ChatManager)->AddMessage(L"퀘스트를 완료했습니다.");
	GET_SINGLE(ChatManager)->SendMessageToServer(::format(L"Quest {0}을 완료함.", questId), false);

	return MakeSendBuffer(pkt, C_QuestFinish);
}

SendBufferRef ClientPacketHandler::Make_C_QuestList()
{
	Protocol::C_QuestList pkt;

	return MakeSendBuffer(pkt, C_QuestList);
}

SendBufferRef ClientPacketHandler::Make_C_Heal(uint64 objectId)
{
	Protocol::C_Heal pkt;

	return MakeSendBuffer(pkt, C_Heal);
}

SendBufferRef ClientPacketHandler::Make_C_AddItem(uint64 objectId, int itemId, int itemCounts, Protocol::ITEM_TYPE itemType, int index)
{
	Protocol::C_AddItem pkt;

	auto* item = pkt.mutable_iteminfo();

	item->set_objectid(objectId);
	item->set_itemid(itemId);
	item->set_itemcount(itemCounts);
	item->set_itemtype(itemType);
	pkt.set_index(index);

	return MakeSendBuffer(pkt, C_AddItem);
}

SendBufferRef ClientPacketHandler::Make_C_EquipItem(uint64 objectId, int itemId, bool equip)
{
	Protocol::C_EquipItem pkt;

	pkt.set_objectid(objectId);
	pkt.set_itemid(itemId);
	pkt.set_equip(equip);

	return MakeSendBuffer(pkt, C_EquipItem);
}

SendBufferRef ClientPacketHandler::Make_C_SyncInventory(uint64 objectID)
{
	Protocol::C_SyncInventory pkt;

	pkt.set_objectid(objectID);

	return MakeSendBuffer(pkt, C_SyncInventory);
}

SendBufferRef ClientPacketHandler::Make_C_KillPlayer(uint64 objectID)
{
	Protocol::C_KillPlayer pkt;

	pkt.set_objectid(objectID);

	return MakeSendBuffer(pkt, C_KillPlayer);
}
