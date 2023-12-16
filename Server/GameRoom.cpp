#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"
#include "Monster.h"

extern GameRoomRef GRoom = make_shared<GameRoom>();

GameRoom::GameRoom()
{

}

GameRoom::~GameRoom()
{

}

void GameRoom::Init()
{
	// 플레이어, 몬스터 소환
	MonsterRef monster = GameObject::CreateMonster();
	monster->info.set_posx(8);
	monster->info.set_posy(8);
	AddObject(monster);

}

void GameRoom::Update()
{

}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = GameObject::CreatePlayer();

	// 클라이언트 서로의 존재를 연결
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	// TEMP
	player->info.set_posx(6);
	player->info.set_posy(6);

	// 입장한 클라이언트에게 정보 전송
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->info);
		session->Send(sendBuffer);
	}

	// 모든 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;
		
		for (auto& item : _players)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}

		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
		session->Send(sendBuffer);
	}

	AddObject(player);
}

void GameRoom::LeaveRoom(GameSessionRef session)
{
	if (session == nullptr)
		return;

	// shared_ptr로 변환 가능한가? -> 아직 존재하는가?
	if (session->player.lock())
		return;

	uint64 id = session->player.lock()->info.objectid();
	RemoveObject(id);
}

GameObjectRef GameRoom::FindObject(uint64 id)
{
	{
		auto findIt = _players.find(id);
		if (findIt != _players.end())
			return findIt->second;
	}
	{
		auto findIt = _monsters.find(id);
		if (findIt != _monsters.end())
			return findIt->second;
	}

	return nullptr;
}

void GameRoom::AddObject(GameObjectRef gameObject)
{
	uint64 id = gameObject->info.objectid();

	auto objectType = gameObject->info.objecttype();

	switch (objectType)
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		_players[id] = static_pointer_cast<Player>(gameObject);
		break;

	case Protocol::OBJECT_TYPE_MONSTER:
		_monsters[id] = static_pointer_cast<Monster>(gameObject);
		break;

	default:
		return;
	}

	gameObject->room = GetRoomRef();

	// 신규 오브젝트 정보를 기존 접속자에게 뿌림
	{
		Protocol::S_AddObject pkt;

		Protocol::ObjectInfo* info = pkt.add_objects();
		*info = gameObject->info;

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
		Broadcast(sendBuffer);
	}

}

void GameRoom::RemoveObject(uint64 id)
{
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	switch (gameObject->info.objecttype())
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		_players.erase(id);
		break;

	case Protocol::OBJECT_TYPE_MONSTER:
		_monsters.erase(id);
		break;

	default:
		return;
	}

	gameObject->room = nullptr;

	// 신규 오브젝트가 삭제 됨을 모두에게 전송
	{
		Protocol::S_RemoveObject pkt;
		pkt.add_ids(id);

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_RemoveObject(pkt);
		Broadcast(sendBuffer);
	}
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
	}
}
