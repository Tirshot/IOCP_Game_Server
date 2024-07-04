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
	srand(time(0));

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
	// ����
	{
		NPCRef npc2 = GameObject::CreateNPC();
		npc2->info.set_posx(40);
		npc2->info.set_posy(25);
		npc2->info.set_defence(9999);
		npc2->info.set_name("Merchant");
		// npc ���� �Է�
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
	// Ʃ�丮��
	{
		NPCRef npc4 = GameObject::CreateNPC();
		npc4->info.set_posx(18);
		npc4->info.set_posy(4);
		npc4->info.set_defence(9999);
		npc4->info.set_name("Tutorial");
		// npc ���� �Է�
		npc4->info.set_npctype(Protocol::NPC_TYPE_TUTORIAL);
		AddObject(npc4);
	}
	{
		GET_SINGLE(Quest)->CreateQuest();
	}

	for (auto& item : _players)
	{
		item.second->Init();
	}

	for (auto& item : _monsters)
	{
		item.second->Init();
	}

	for (auto& item : _npcs)
	{
		item.second->BeginPlay();
	}

	for (auto& item : _items)
	{
		item.second->BeginPlay();
	}

	for (auto& item : _triggers)
	{
		item.second->BeginPlay();
	}

	for (auto& inven : _inventorys)
	{
		inven.second->BeginPlay();
	}

	for (auto& item : _projectiles)
	{
		item.second->BeginPlay();
	}
}

void GameRoom::Update()
{
	RandomMonsterSpawn();

	for (auto& item : _players)
	{
		item.second->Update();

		auto id = item.second->GetObjectID();
		// ��򰡿� ���߽� ����
		if (item.second->info.hp() <= 0)
		{
			_temps[id] = item.second;
		}
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
		// ����� ����
		if (item.second->IsGet())
		{
			_deleteObjects[id] = item.second;
			wstring itemStr = to_wstring(id);

			// ���� �� �α�
			GChat->AddText(L"ItemID : " + itemStr + L" ������ �������� ����.");
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

	for (auto& item : _projectiles)
	{
		item.second->Tick();

		auto id = item.second -> GetObjectID();
		// ��򰡿� ���߽� ����
		if (item.second->IsHit())
		{
			_deleteObjects[id] = item.second;
		}
	}
	// ����ü �� ������ ����
	for (const auto& del : _deleteObjects)
	{
		if (del.second->GetType() == Protocol::OBJECT_TYPE_PROJECTILE)
			_projectiles.erase(del.first);

		if (del.second->GetType() == Protocol::OBJECT_TYPE_ITEM)
			_items.erase(del.first);
	}
	_deleteObjects.clear();
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	// �÷��̾� �ʱ�ȭ
	auto player = GameObject::CreatePlayer();

	InventoryRef inventory = GameObject::CreateInventory(player);
	AddObject(inventory);

	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	SetName(player);

	// Ŭ���̾�Ʈ ������ ���縦 ����
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	GChat->AddText(::format(L"System : Player {0} ���� ����.", player->GetObjectID()));

	// Player Character Spawn
	Vec2Int randCellPos = GetRandomEmptySpawnCellPos();
	player->info.set_posx(5);
	player->info.set_posy(5);
	player->info.set_dir(Protocol::DIR_TYPE_DOWN);

	// ������ Ŭ���̾�Ʈ���� ���� ����
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->info);
		session->Send(sendBuffer);
	}

	// ��� ������Ʈ ���� ����
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

		for (auto& item : _projectiles)
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

	// shared_ptr�� ��ȯ �����Ѱ�? -> ���� �����ϴ°�?
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
		auto findIt = _projectiles.find(id);
		if (findIt != _projectiles.end())
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

void GameRoom::SetName(PlayerRef player)
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
		_projectiles[id] = static_pointer_cast<Projectile>(gameObject);
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

	// �ű� ������Ʈ ������ ���� �����ڿ��� �Ѹ�
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
		_deleteObjects[id] = gameObject;
		break;

	case Protocol::OBJECT_TYPE_ITEM:
		_deleteObjects[id] = gameObject;
		break;

	default:
		return;
	}

	gameObject->room = nullptr;

	// �ű� ������Ʈ�� ���� ���� ��ο��� ����
	{
		Protocol::S_RemoveObject pkt;
		pkt.add_ids(id);

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_RemoveObject(pkt);
		Broadcast(sendBuffer);
	}
}

void GameRoom::RemoveTemp(uint64 id)
{
	//GameObjectRef gameObject = FindObject(id);
	//if (gameObject == nullptr)
	//	return;

	_temps.erase(id);
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	// �ڱ� �ڽ��� ������ ��� �÷��̾ ������� Broadcast
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
	}
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer, uint64 objectId)
{
	// Ư�� ID�� ������ Broadcast
	for (auto& item : _players)
	{
		if (item.second->GetObjectID() == objectId)
			continue;

		item.second->session->Send(sendBuffer);
	}
}

PlayerRef GameRoom::FindClosestPlayer(Vec2Int cellPos)
{
	float best = FLT_MAX;
	PlayerRef ret = nullptr;

	for (const auto& item :_players)
	{
		const auto& player = item.second;
		if (player)
		{
			// �÷��̾ ���� ������ ������ Ÿ�������� ����
			if (player->IsInSafeZone())
			{
				ret = nullptr;
				continue;
			}

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
	// �� ������ ã�� ���ϵ��� ����ó��
	int32 depth = abs(src.y - dest.y) + abs(src.x - dest.x);
	if (depth >= maxDepth)
		return false;

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	map<Vec2Int, int32> best;
	map<Vec2Int, Vec2Int> parent;

	// �ʱⰪ
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
		// ���� ���� �ĺ� ã��
		PQNode node = pq.top();
		pq.pop();

		// �� ª�� ��θ� �ڴʰ� ã������ ��ŵ
		if (best[node.pos] < node.cost)
			continue;

		// ������ ���� �� ����
		if (node.pos == dest)
		{
			found = true;
			break;
		}

		// �湮
		for (int32 dir = 0; dir < 4; dir++)
		{
			// �����¿�� ���� ��ǥ�� Ž��
			Vec2Int nextPos = node.pos + front[dir];

			if (CanGo(nextPos) == false)
				continue;

			// �� ������ ã�� ���ϵ��� ����ó��
			int32 depth = abs(nextPos.y - src.y) + abs(nextPos.x - src.x);
			if (depth >= maxDepth)
				continue;

			int32 cost = abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x);
			int32 bestCost = best[nextPos];
			if (bestCost != 0)
			{
				// �ٸ� ��ο��� �� ���� ���� ã�Ҵٸ� ��ŵ
				if (bestCost <= cost)
					continue;
			}

			// ����
			best[nextPos] = cost;
			pq.push(PQNode(cost, nextPos));
			parent[nextPos] = node.pos;
		}
	}
	// ���� �����ִ��� Ȯ��
	if (found == false)
	{
		// ���� ã�� �������� ���� ������ �̵��� �� ����
		float bestScore = FLT_MAX;

		for (auto& item : best)
		{
			Vec2Int pos = item.first;
			int32 score = item.second;

			// �����̸� ���� ��ġ���� �� �̵��ϴ� ������
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

	// ���������� �Ž��� �ö�
	while (true)
	{
		path.push_back(pos);

		// ������
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

	// ���� �浹
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	return tile->value != 1;
}

bool GameRoom::MonsterCanGo(Vec2Int cellPos)
{
	Tile* tile = _tilemap.GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// ���� �浹
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	if (tile->value == 2)
		return false;

	return tile->value == 0;
}

bool GameRoom::IsSafeZone(Vec2Int cellPos)
{
	Tile* tile = _tilemap.GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	if (tile->value != 2)
		return false;

	return tile->value == 2;
}

Vec2Int GameRoom::GetRandomEmptyCellPos()
{
	Vec2Int ret = { -1,-1 };

	Vec2Int size = _tilemap.GetMapSize();

	// ������ �������� ������������ �����ؾ� ��
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

	// ������ �������� ������������ �����ؾ� ��
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
	for (const auto& item : _players)
	{
		const auto& player = item.second;
		if (player)
		{
			auto playerCellPos = player->GetCellPos();

			if (playerCellPos == cellPos)
			{
				return player;
			}
		}
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

bool GameRoom::IsOtherMonstersAround(MonsterRef monster)
{
	// �ش� ���� ���� 3ĭ �Ÿ��� �ٸ� ���Ͱ� �����ϴ°�
	for (auto& item : _monsters)
	{
		// �ڱ� �ڽ��� �ǳʶٱ�
		if (item.second == monster)
			continue;

		auto other = item.second;
		auto otherPos = other->GetCellPos();
		auto pos = monster->GetCellPos();

		auto vec = pos - otherPos;
		if (vec.Length() <= 3)
			return true;
	}
	return false;
}

void GameRoom::RandomMonsterSpawn()
{
	// �� 0.5�ʸ��� ������ ���� ��ŭ�� ����
	if (_monsterCount > DESIRED_MONSTER_COUNT)
		return;

	Vec2Int randPos = GetRandomEmptyCellPos();

	// Ȯ���� ���� ��(����ġ 30), ���(10), ��Ÿ��(20) ���� ����
	auto randValue = rand() % 60 + 1;

	if (randValue <= 30)
	{
		auto snake = GameObject::CreateMonster(Protocol::MONSTER_TYPE_SNAKE);
		while (true)
		{
			snake->SetCellPos(randPos, true);
			// �Ÿ� 3 �̳��� �ٸ� ���Ͱ� ������ ���ġ
			if (IsOtherMonstersAround(snake))
			{
				randPos = GetRandomEmptyCellPos();
			}
			else
			{
				break;
			}
		}
		{
			AddObject(snake);
			_monsterCount++;
			GChat->AddText(::format(L"snake {0} ����.", snake->info.objectid()));
		}
	}
	else if (randValue > 30 && randValue <= 40)
	{
		auto moblin = GameObject::CreateMonster(Protocol::MONSTER_TYPE_MOBLIN);
		while (true)
		{
			moblin->SetCellPos(randPos, true);
			// �Ÿ� 3 �̳��� �ٸ� ���Ͱ� ������ ���ġ
			if (IsOtherMonstersAround(moblin))
			{
				randPos = GetRandomEmptyCellPos();
			}
			else
			{
				break;
			}
		}
		AddObject(moblin);
		_monsterCount++;
		GChat->AddText(::format(L"moblin {0} ����.", moblin->info.objectid()));
	}
	else if (randValue > 40 && randValue <= 60)
	{
		auto octoroc = GameObject::CreateMonster(Protocol::MONSTER_TYPE_OCTOROC);
		while (true)
		{
			octoroc->SetCellPos(randPos, true);
			// �Ÿ� 3 �̳��� �ٸ� ���Ͱ� ������ ���ġ
			if (IsOtherMonstersAround(octoroc))
			{
				randPos = GetRandomEmptyCellPos();
			}
			else
			{
				break;
			}
		}
		AddObject(octoroc);
		_monsterCount++;
		GChat->AddText(::format(L"octoroc {0} ����.", octoroc->info.objectid()));
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
	// �̵� ��Ŷ�� �޾��� �� ó��
	uint64 id = pkt.info().objectid();
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	gameObject->info = pkt.info();

	// Ŭ���̾�Ʈ�� ��Ŷ�� ��ε�ĳ��Ʈ
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
