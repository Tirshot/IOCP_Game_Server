#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "Player.h"

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

SendBufferRef ServerPacketHandler::Make_S_SendChat(uint64 objectId, uint64 time, string str)
{
	Protocol::S_SendChat pkt;
	auto* texts = pkt.mutable_texts();
	texts->set_objectid(objectId);
	texts->set_time(time);
	texts->set_str(str);

	return MakeSendBuffer(pkt, S_SendChat);
}

//SendBufferRef ServerPacketHandler::Make_S_Hit(uint64 objectId, uint64 attackerId)
//{
//	Protocol::S_Hit pkt;
//
//	pkt.set_objectid(objectId);
//	pkt.set_attackerid(attackerId);
//
//	return MakeSendBuffer(pkt, S_Hit);
//}

SendBufferRef ServerPacketHandler::Make_S_Fire(const Protocol::ObjectInfo& info, uint64 id)
{
	Protocol::S_Fire pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	pkt.set_ownerid(id);

	return MakeSendBuffer(pkt, S_Fire);
}
