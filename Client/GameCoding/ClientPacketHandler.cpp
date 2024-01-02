#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "Game.h"
#include "Monster.h"
#include "HitEffect.h"
#include "Creature.h"
#include "Player.h"
#include "ChatManager.h"
#include "Chat.h"

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

		case S_Fire:
			Handle_S_Fire(session, buffer, len);
			break;

		case S_SendChat:
			Handle_S_SendChat(session, buffer, len);
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

	// 마이 플레이어 배치 및 정보 동기화
	const Protocol::ObjectInfo& info = pkt.info();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		MyPlayer* myPlayer = scene->SpawnObject<MyPlayer>(Vec2Int{ info.posx(), info.posy() });
		myPlayer->info = info;
		myPlayer->SetDir(info.dir());
		myPlayer->SetState(info.state());
		GET_SINGLE(SceneManager)->SetMyPlayer(myPlayer);
	}
}

void ClientPacketHandler::Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_AddObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
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

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		scene->Handle_S_RemoveObject(pkt);
}

void ClientPacketHandler::Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::ObjectInfo& info = pkt.info();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
		if (myPlayerId == info.objectid())
			return;

		GameObject* gameObject = scene->GetObjects(info.objectid());
		if (gameObject)
		{
			gameObject->SetDir(info.dir());
			gameObject->SetState(info.state());
			gameObject->SetCellPos(Vec2Int{ info.posx(), info.posy() });
			gameObject->SetWeaponType(info.weapontype());
		}
	}
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

	
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	// 화살의 주인 플레이어를 가져옴
	if (scene)
	{
		GameObject* gameObject = scene->GetObjects(pkt.ownerid());
		Player* player = static_cast<Player*>(gameObject);
		if (player)
		{
			player->Handle_S_Fire(info, pkt.ownerid());
		}
	}
}

void ClientPacketHandler::Handle_S_SendChat(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_SendChat pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::Text& texts = pkt.texts();
	uint64 objectId = texts.objectid();

	time_t now = texts.time();
	wstring nowTime = GET_SINGLE(ChatManager)->ChangeTimeFormat(now);

	string str = texts.str();
	wstring str2 = GET_SINGLE(ChatManager)->StringToWStr(str);

	// 본인은 본인의 메세지만 볼 수 있음
	if (GET_SINGLE(SceneManager)->GetMyPlayerId() != texts.objectid())
		return;

	Chat* chat = GET_SINGLE(ChatManager)->GetChat();

	chat->AddText(format(L"[{0}] {1}", nowTime, str2));
}

//void ClientPacketHandler::Handle_S_Hit(ServerSessionRef session, BYTE* buffer, int32 len)
//{
//	PacketHeader* header = (PacketHeader*)buffer;
//	//uint16 id = header->id;
//	uint16 size = header->size;
//	Protocol::S_Hit pkt;
//	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
//	//
//	uint64 objectId = pkt.objectid();
//	uint64 attackerId = pkt.attackerid();
//
//	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
//
//	if (scene)
//	{
//		GameObject* object = scene->GetObjects(objectId);
//		GameObject* attackerObject = scene->GetObjects(attackerId);
//
//		Creature* creature = dynamic_cast<Creature*>(object);
//		Creature* attacker = dynamic_cast<Creature*>(attackerObject);
//
//		creature->Handle_S_Hit(attacker);
//		
//	}
//}

// 패킷 보내기
SendBufferRef ClientPacketHandler::Make_C_Move()
{
	// 패킷 생성
	Protocol::C_Move pkt;

	// MyPlayer를 가져옴
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	// 패킷의 info를 수정해 myPlayer의 정보를 패킷에 담음
	*pkt.mutable_info() = myPlayer->info;
	
	return MakeSendBuffer(pkt, C_Move);
}

SendBufferRef ClientPacketHandler::Make_C_Fire(uint64 ownerid)
{
	// 패킷 생성
	Protocol::C_Fire pkt;

	pkt.set_ownerid(ownerid);

	return MakeSendBuffer(pkt, C_Fire);
}

SendBufferRef ClientPacketHandler::Make_C_Hit(Protocol::ObjectInfo& objectInfo, uint64 attackerId)
{
	// 패킷 생성
	Protocol::C_Hit pkt;
	*pkt.mutable_info() = objectInfo;
	pkt.set_attackerid(attackerId);

	return MakeSendBuffer(pkt, C_Hit);
}

//SendBufferRef ClientPacketHandler::Make_C_FireArrow()
//{
	// 패킷 생성

	// MyPlayer를 가져옴
	//Arrow* arrow = GET_SINGLE(SceneManager)->GetMyPlayer()->

	//// 패킷의 weapontype을 수정해 myPlayer의 정보를 패킷에 담음
	//*pkt.mutable_info() = myPlayer->info;

	//return MakeSendBuffer(pkt, C_WeaponChange);

//}

