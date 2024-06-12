#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Sign.h"
#include "Item.h"
#include "Arrow.h"
#include "Chat.h"
#include "Quest.h"
#include "Inventory.h"
#include "Trigger.h"
#include "Quest1Trigger.h"
#include "ItemManager.h"
#include <filesystem>

extern GameRoomRef GRoom = make_shared<GameRoom>();

GameRoom::GameRoom()
{

}

GameRoom::~GameRoom()
{
	cout << "~GameRoom" << endl;
}

void GameRoom::Init()
{
	filesystem::path currentPath = filesystem::current_path();
	filesystem::path tilemapDirectory = currentPath / ".." / "Client" / "Resources" / "Tilemap" / "Tilemap01.txt";
	filesystem::path relativePath = filesystem::relative(tilemapDirectory, currentPath);
	_tilemap.LoadFile(relativePath);
	GET_SINGLE(Quest)->Init();
	GET_SINGLE(ItemManager)->Init();
	// Sign
	{
		SignRef npc1 = GameObject::CreateSign();
		npc1->info.set_posx(4);
		npc1->info.set_posy(3);
		npc1->info.set_defence(9999);
		npc1->info.set_name("Sign");
		AddObject(npc1);
	}
	// 상인
	{
		NPCRef npc2 = GameObject::CreateNPC();
		npc2->info.set_posx(40);
		npc2->info.set_posy(25);
		npc2->info.set_defence(9999);
		npc2->info.set_name("Merchant");
		// npc 종류 입력
		npc2->info.set_npctype(Protocol::NPC_TYPE_MERCHANT);
		AddObject(npc2);
	}
	// Sign
	{
		SignRef npc3 = GameObject::CreateSign();
		npc3->info.set_posx(28);
		npc3->info.set_posy(4);
		npc3->info.set_defence(9999);
		npc3->info.set_name("Merchant_Sign");
		AddObject(npc3);
	}
	{
		GET_SINGLE(Quest)->CreateQuest();
	}
}

void GameRoom::Update()
{
	srand(time(0));

	RandomMonsterSpawn();

	for (auto& item : _players)
	{
		item.second->Update();
	}

	for (auto& item : _monsters)
	{
		item.second->Update();
	}

	for (auto& item : _npcs)
	{
		item.second->Tick();
	}

	for (auto& item : _items)
	{
		item.second->Tick();

		auto id = item.second->GetObjectID();
		// 습득시 제거
		if (item.second->IsGet())
		{
			_deleteObjects[id] = item.second;

			int itemID = item.second.get()->GetItemInfo().itemid();

			wstring itemStr = to_wstring(itemID);

			// 제거 후 로그
			GChat->AddText(L"ItemID : " + itemStr + L" 아이템 서버에서 제거.");
		}
	}

	for (auto& item : _triggers)
	{
		item.second->Tick();
	}

	for (auto& inven : _inventorys)
	{
		inven.second->Update();
	}

	for (auto& item : _arrows)
	{
		item.second->Tick();

		auto id = item.second -> GetObjectID();
		// 어딘가에 적중시 제거
		if (item.second->IsHit())
		{
			_deleteObjects[id] = item.second;
		}
	}
	// 투사체 및 아이템 제거
	for (const auto& del : _deleteObjects)
	{
		if (del.second->GetType() == Protocol::OBJECT_TYPE_PROJECTILE)
			_arrows.erase(del.first);

		if (del.second->GetType() == Protocol::OBJECT_TYPE_ITEM)
			_items.erase(del.first);
	}
	_deleteObjects.clear();
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	// 플레이어 초기화
	PlayerRef player = GameObject::CreatePlayer();
	InventoryRef inventory = GameObject::CreateInventory(player);
	AddObject(inventory);

	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	SetName(player);

	// 클라이언트 서로의 존재를 연결
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	GChat->AddText(::format(L"System : Player {0} 게임 입장.", player->GetObjectID()));

	// Player Character Spawn
	Vec2Int randCellPos = GetRandomEmptySpawnCellPos();
	player->info.set_posx(5);
	player->info.set_posy(5);
	player->info.set_dir(Protocol::DIR_TYPE_DOWN);

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

		for (auto& item : _npcs)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}

		for (auto& item : _arrows)
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
	if (session->player.lock() == nullptr)
		return;

	uint64 id = session->player.lock()->info.objectid();
	RemoveObject(id);

	GChat->SaveLogFile();
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
	{
		auto findIt = _npcs.find(id);
		if (findIt != _npcs.end())
			return findIt->second;
	}
	{
		auto findIt = _arrows.find(id);
		if (findIt != _arrows.end())
			return findIt->second;
	}
	{
		auto findIt = _items.find(id);
		if (findIt != _items.end())
			return findIt->second;
	}
	return nullptr;
}

weak_ptr<Player> GameRoom::FindObjectInTemp(uint64 id)
{
	auto findIt = _temps.find(id);
	if (findIt != _temps.end())
		return findIt->second;

	return {};
}

void GameRoom::SetName(PlayerRef& player)
{
	string strPlayer = "Player";
	string str = strPlayer.append(to_string(player->GetObjectID()));
	player->info.set_name(str);
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

	case Protocol::OBJECT_TYPE_NPC:
		_npcs[id] = static_pointer_cast<NPC>(gameObject);
		break;

	case Protocol::OBJECT_TYPE_NPC_SIGN:
		_npcs[id] = static_pointer_cast<NPC>(gameObject);
		break;

	case Protocol::OBJECT_TYPE_PROJECTILE:
		_arrows[id] = static_pointer_cast<Arrow>(gameObject);
		return;

	case Protocol::OBJECT_TYPE_ITEM:
		_items[id] = static_pointer_cast<Item>(gameObject);
		return;

	case Protocol::OBJECT_TYPE_INVENTORY:
		_inventorys[id] = static_pointer_cast<Inventory>(gameObject);
		return;

	case Protocol::OBJECT_TYPE_TRIGGER:
		_triggers[id] = static_pointer_cast<Trigger>(gameObject);
		return;

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
		_temps[id] = _players[id];
		_players.erase(id);
		break;

	case Protocol::OBJECT_TYPE_MONSTER:
		_monsters.erase(id);
		_monsterCount--;
		break;

	case Protocol::OBJECT_TYPE_NPC:
		_npcs.erase(id);
		break;

	case Protocol::OBJECT_TYPE_PROJECTILE:
		_arrows.erase(id);
		break;

	case Protocol::OBJECT_TYPE_ITEM:
		_items.erase(id);
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

void GameRoom::RemoveTemp(uint64 id)
{
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	_temps.erase(id);
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
	}
}

PlayerRef GameRoom::FindClosestPlayer(Vec2Int cellPos)
{
	float best = FLT_MAX;
	PlayerRef ret = nullptr;

	for (auto& item :_players)
	{
		PlayerRef player = item.second;
		if (player)
		{
			Vec2Int dir = cellPos - player->GetCellPos();
			float dist = dir.LengthSquared();
			if (dist < best)
			{
				best = dist;
				ret = player;
			}
		}
	}
	return ret;
}

bool GameRoom::FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth)
{
	// 맵 끝까지 찾지 못하도록 예외처리
	int32 depth = abs(src.y - dest.y) + abs(src.x - dest.x);
	if (depth >= maxDepth)
		return false;

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	map<Vec2Int, int32> best;
	map<Vec2Int, Vec2Int> parent;

	// 초기값
	{
		int32 cost = abs(dest.y - src.y) + abs(dest.x - src.x);

		pq.push(PQNode(cost, src));
		best[src] = cost;
		parent[src] = src;
	}

	Vec2Int front[4] =
	{
		{0,-1},
		{0,1},
		{-1,0},
		{1,0}
	};

	bool found = false;

	while (pq.empty() == false)
	{
		// 제일 좋은 후보 찾기
		PQNode node = pq.top();
		pq.pop();

		// 더 짧은 경로를 뒤늦게 찾았으면 스킵
		if (best[node.pos] < node.cost)
			continue;

		// 목적지 도달 시 종료
		if (node.pos == dest)
		{
			found = true;
			break;
		}

		// 방문
		for (int32 dir = 0; dir < 4; dir++)
		{
			// 상하좌우로 다음 좌표를 탐색
			Vec2Int nextPos = node.pos + front[dir];

			if (CanGo(nextPos) == false)
				continue;

			// 맵 끝까지 찾지 못하도록 예외처리
			int32 depth = abs(nextPos.y - src.y) + abs(nextPos.x - src.x);
			if (depth >= maxDepth)
				continue;

			int32 cost = abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x);
			int32 bestCost = best[nextPos];
			if (bestCost != 0)
			{
				// 다른 경로에서 더 빠른 길을 찾았다면 스킵
				if (bestCost <= cost)
					continue;
			}

			// 예약
			best[nextPos] = cost;
			pq.push(PQNode(cost, nextPos));
			parent[nextPos] = node.pos;
		}
	}
	// 길이 막혀있는지 확인
	if (found == false)
	{
		// 길을 찾지 못했으나 가장 가까이 이동할 수 있음
		float bestScore = FLT_MAX;

		for (auto& item : best)
		{
			Vec2Int pos = item.first;
			int32 score = item.second;

			// 동점이면 최초 위치에서 덜 이동하는 쪽으로
			if (bestScore == score)
			{
				int32 dist1 = abs(dest.x - src.x) + abs(dest.y - src.y);
				int32 dist2 = abs(pos.x - src.x) + abs(pos.y - src.y);
				if (dist1 > dist2)
					dest = pos;
			}
			if (bestScore > score)
			{
				dest = pos;
				bestScore = score;
			}
		}
	}
	path.clear();
	Vec2Int pos = dest;

	// 도착점에서 거슬러 올라감
	while (true)
	{
		path.push_back(pos);

		// 시작점
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(path.begin(), path.end());
	return true;
}

bool GameRoom::CanGo(Vec2Int cellPos)
{
	Tile* tile = _tilemap.GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// 몬스터 충돌
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	return tile->value != 1;
}

bool GameRoom::MonsterCanGo(Vec2Int cellPos)
{
	Tile* tile = _tilemap.GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// 몬스터 충돌
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	if (tile->value == 2)
		return false;

	return tile->value == 0;
}

Vec2Int GameRoom::GetRandomEmptyCellPos()
{
	Vec2Int ret = { -1,-1 };

	Vec2Int size = _tilemap.GetMapSize();

	// 굉장히 많아지면 빠져나오도록 설정해야 함
	while (true)
	{
		int32 x = rand() % size.x;
		int32 y = rand() % size.y;
		Vec2Int cellPos{ x, y };

		if (MonsterCanGo(cellPos))
			return cellPos;
	}
}

Vec2Int GameRoom::GetRandomEmptySpawnCellPos()
{
	Vec2Int ret = { -1,-1 };
	Vec2Int size = Vec2Int{ 8, 8 };

	// 굉장히 많아지면 빠져나오도록 설정해야 함
	while (true)
	{
		int32 x = rand() % size.x;
		int32 y = rand() % size.y;
		Vec2Int cellPos{ x, y };

		if (CanGo(cellPos))
			return cellPos;
	}
}

CreatureRef GameRoom::GetCreatureAt(Vec2Int cellPos)
{
	for (auto& item : _players)
	{
		if (item.second->GetCellPos() == cellPos)
			return item.second;
	}

	for (auto& item : _monsters)
	{
		if (item.second->GetCellPos() == cellPos)
			return item.second;
	}

	for (auto& item : _npcs)
	{
		if (item.second->GetCellPos() == cellPos)
			return item.second;
	}
	return nullptr;
}

void GameRoom::RandomMonsterSpawn()
{
	// 정해진 숫자 만큼만 스폰
	if (_monsterCount > DESIRED_MONSTER_COUNT)
		return;

	Vec2Int randPos = GetRandomEmptyCellPos();
	//
	{
		auto monster = GameObject::CreateMonster(Protocol::MONSTER_TYPE_SNAKE);
		monster->SetCellPos(randPos, true);
		AddObject(monster);
		_monsterCount++;
	}
}

Protocol::QuestInfo& GameRoom::GetQuest(int questId)
{
	auto findit = _quests.find(questId);
	if (findit != _quests.end())
	{
		Protocol::QuestInfo& info = _quests[questId];
		return info;
	}
}

void GameRoom::Handle_C_Move(Protocol::C_Move& pkt)
{
	// 이동 패킷을 받았을 때 처리
	uint64 id = pkt.info().objectid();
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	// set state, dir, pos + weaponType

	gameObject->info = pkt.info();

	// 클라이언트의 패킷을 브로드캐스트
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(pkt.info());
		Broadcast(sendBuffer);
	}
}

void GameRoom::AddQuest(Quest quest)
{
	_quests.insert({ quest.info.questid(), quest.info });
}

void GameRoom::AddItemToPlayer(int objectId, int itemId, int itemCounts, Protocol::ITEM_TYPE itemType, int index)
{
	GetInventory(objectId)->AddItemToInventory(itemId, itemCounts, itemType, index);
}

void GameRoom::EquipItemToPlayer(int objectId, int itemId, bool equip)
{
	GetInventory(objectId)->EquipItem(itemId, equip);
}

void GameRoom::SetQuestStates(uint64 objectId, int questId, Protocol::QUEST_STATE state)
{
	_questsStates[objectId][questId].state = state;
}

map<int, PlayerQuestState> GameRoom::GetQuestsStatesByID(uint64 objectId)
{
	return _questsStates[objectId];
}

void GameRoom::SetQuestStateProgress(uint64 objectId, int questId, int progress)
{
	_questsStates[objectId][questId].progress = progress;
}
