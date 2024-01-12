#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Arrow.h"

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

	case C_Teleport:
		Handle_C_Teleport(session, buffer, len);
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
 	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Hit pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	uint64 objectId = pkt.info().objectid();
	uint64 attackerId = pkt.attackerid();

	// OnDamaged 함수는 피격자가 주체
	GameRoomRef room = session->gameRoom.lock();
	if (room)
	{
		GameObjectRef gameObject = room->FindObject(objectId);
		CreatureRef creature = dynamic_pointer_cast<Creature>(gameObject);

		GameObjectRef attackerObject = room->FindObject(attackerId);
		CreatureRef attacker = dynamic_pointer_cast<Creature>(attackerObject);

		if (creature && attacker)
		{
			creature->info.set_hp(pkt.info().hp());
			creature->OnDamaged(attacker);
		}
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
		GameObjectRef object = room->FindObject(pkt.ownerid());

		if (object)
		{
			PlayerRef player = static_pointer_cast<Player>(object);
			// 화살발사?
			player->MakeArrow();
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

	GameRoomRef room = session->gameRoom.lock();
	if (room)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendMessage(objectId, now, str);
		room->Broadcast(sendBuffer);
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
		player->info.set_hp(info.maxhp());
		player->info.set_posx(5);
		player->info.set_posy(5);
		player->info.set_weapontype(Protocol::WEAPON_TYPE_SWORD);

		// 클라이언트 서로의 존재를 연결
		session->gameRoom = room;
		session->player = player;
		player->session = session;
		player->info.set_dir(Protocol::DIR_TYPE_DOWN);

		// 입장한 클라이언트에게 정보 전송
		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->info);
			session->Send(sendBuffer);
		}

		// 모든 오브젝트 정보 전송
		{
			Protocol::S_AddObject pkt;

			for (auto& item : room->GetPlayers())
			{
				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;
			}

			for (auto& item : room->GetMonsters())
			{
				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;
			}

			for (auto& item : room->GetNPCs())
			{
				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;
			}

			for (auto& item : room->GetArrows())
			{
				Protocol::ObjectInfo* info = pkt.add_objects();
				*info = item.second->info;
			}

			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
			session->Send(sendBuffer);
		}
		room->AddObject(player);
		room->RemoveTemp(id);
	}
}

void ServerPacketHandler::Handle_C_Teleport(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Teleport pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	// session->gameRoom
	GameRoomRef room = session->gameRoom.lock();
	if (room)
		room->Handle_C_Teleport(pkt.objectid());
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

SendBufferRef ServerPacketHandler::Make_S_EnterGame()
{
	Protocol::S_EnterGame pkt;

	pkt.set_success(true);
	pkt.set_accountid(0); // DB에서 가져옴

	// Searialize
	return MakeSendBuffer(pkt, S_EnterGame);
}

SendBufferRef ServerPacketHandler::Make_S_MyPlayer(const Protocol::ObjectInfo& info)
{
	Protocol::S_MyPlayer pkt;

	// Struct 내의 Struct -> mutable_info, 포인터 반환
	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	return MakeSendBuffer(pkt, S_MyPlayer);
}

SendBufferRef ServerPacketHandler::Make_S_AddObject(const Protocol::S_AddObject& pkt)
{
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

SendBufferRef ServerPacketHandler::Make_S_Fire(const Protocol::ObjectInfo& info, uint64 id)
{
	Protocol::S_Fire pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	pkt.set_ownerid(id);

	return MakeSendBuffer(pkt, S_Fire);
}
