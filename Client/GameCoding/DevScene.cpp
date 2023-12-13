#include "pch.h"
#include "DevScene.h"
#include "Utils.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "SpriteActor.h"
#include "Flipbook.h"
#include "Player.h"
#include "UI.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "SoundManager.h"
#include "Sound.h"
#include "Monster.h"

DevScene::DevScene()
{
	printf("DevMode");
}

DevScene::~DevScene()
{

}

void DevScene::Init()
{
	// ��� ���ҽ��� Scene���� �ҷ��´�
	// �ٸ� Scene���� �Ѿ �� ����(Clear)�Ѵ�
	// �������� 01
	GET_SINGLE(ResourceManager)->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");

	// Ÿ��
	GET_SINGLE(ResourceManager)->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Potion", L"Sprite\\UI\\Mp.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0,0,0));
	
	GET_SINGLE(ResourceManager)->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp");

	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01"));
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileO", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 0, 0 ,48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileX", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 48, 0, 48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Start"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_On", GET_SINGLE(ResourceManager)->GetTexture(L"Start"), 150, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Edit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Edit"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Edit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Edit"), 150, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Exit"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Exit"), 150, 0, 150, 150);

	LoadMap();
	LoadPlayer();
	LoadMonster();
	LoadProjectile();
	LoadEffect();
	LoadTilemap();

	SpawnObject<Player>(Vec2Int {5,5});
	SpawnObject<Monster>(Vec2Int {7,7});
	
	Super::Init();
}

void DevScene::Update()
{
	Super::Update();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	TickMonsterSpawn();
}

void DevScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

void DevScene::AddActor(Actor* actor)
{
	Super::AddActor(actor);

	Monster* monster = dynamic_cast<Monster*>(actor);
	if (monster)
	{
		_monsterCount++;
	}
}

void DevScene::RemoveActor(Actor* actor)
{
	Super::RemoveActor(actor);

	Monster* monster = dynamic_cast<Monster*>(actor);
	if (monster)
	{
		_monsterCount--;
	}
}

void DevScene::Clear()
{
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			SAFE_DELETE(actor);

	for (vector<Actor*>& actors : _actors)
		actors.clear();

	for (UI* ui : _uis)
		SAFE_DELETE(ui);
}

void DevScene::LoadMap()
{
	// ���
	Sprite* sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Stage01");

	SpriteActor* background = new SpriteActor();
	background->SetSprite(sprite);
	background->SetLayer(LAYER_BACKGROUND);

	const Vec2Int size = sprite->GetSize();
	background->SetPos(Vec2((float)size.x / 2, (float)size.y / 2));

	AddActor(background);
}

void DevScene::LoadPlayer()
{
	// �÷��̾� Idle
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveUp", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveDown", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveLeft", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveRight", { 200, 200}, 0, 9, 0, 0.5f });
	}
	// �÷��̾� Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveUp", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveDown", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveLeft", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveRight", { 200, 200}, 0, 9, 1, 0.5f });
	}
	// �÷��̾� Attack, �Ű����� ���� loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackUp", { 200, 200}, 0, 7, 3, 0.3f , false});
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackDown", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackLeft", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackRight", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	// �÷��̾� Bow, �Ű����� ���� loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowUp", { 200, 200}, 0, 5, 5, 0.3f , false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowDown", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowLeft", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowRight", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	// �÷��̾� Staff, �Ű����� ���� loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffUp", { 200, 200}, 0, 10, 6, 0.5f , false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffDown", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffLeft", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffRight", { 200, 200}, 0, 10, 6, 0.5f, false });
	}


}

void DevScene::LoadMonster()
{
	// Snake Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 3, 3, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 0, 3, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 0, 3, 2, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 0, 3, 1, 0.5f });
	}
}

void DevScene::LoadProjectile()
{
	// Arrow Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowUp", { 100, 100}, 0, 0, 3, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowDown", { 100, 100}, 0, 0, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowLeft", { 100, 100}, 0, 0, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowRight", { 100, 100}, 0, 0, 2, 0.5f });
	}
}

void DevScene::LoadEffect()
{
	// HitEffect
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Hit");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Hit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_Hit", { 50, 47}, 0, 5, 0, 0.3f, false });
	}
}

void DevScene::LoadTilemap()
{
	// Ÿ�ϸ�
		TilemapActor* actor = new TilemapActor();
		AddActor(actor);

		_tilemapActor = actor;
		{
			auto* tm = GET_SINGLE(ResourceManager)->CreateTilemap(L"Tilemap_01");
			tm->SetMapSize({ 63,43 });
			tm->SetTileSize(48);

			GET_SINGLE(ResourceManager)->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");
			_tilemapActor->SetTilemap(tm);
			_tilemapActor->SetShowDebug(false);
		}
}

Player* DevScene::FindClosestPlayer(Vec2Int cellPos)
{
	float best = FLT_MAX;
	Player* ret = nullptr;

	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		Player* player = dynamic_cast<Player*>(actor);
		if (player)
		{
			Vec2Int dir = cellPos - player->GetCellPos();
			float dist = dir.LengthSquared();
			if (dist < best)
			{
				dist = best;
				ret = player;
			}
		}
	}
	return ret;
}

bool DevScene::FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth /*= 10*/)
{
	// �� ������ ã�� ���ϵ��� ����ó��
	int32 depth = abs(dest.y - src.y) + abs(dest.x - src.x);
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

bool DevScene::CanGo(Vec2Int cellPos)
{
	if (_tilemapActor == nullptr)
		return false;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return false;

	Tile* tile = tm->GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// ���� �浹
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	return tile->value != 1;
}

Vec2 DevScene::ConvertPos(Vec2Int cellPos)
{
	Vec2 ret = {};

	if (_tilemapActor == nullptr)
		return ret;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return ret;

	int32 size = tm->GetTileSize();
	Vec2 pos = _tilemapActor->GetPos();

	ret.x = pos.x + cellPos.x * size + (size / 2);
	ret.y = pos.y + cellPos.y * size + (size / 2);

	return ret;
}

Vec2Int DevScene::GetRandomEmptyCellPos()
{
	Vec2Int ret = { -1,-1 };
	if (_tilemapActor == nullptr)
		return ret;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return ret;

	Vec2Int size = tm->GetMapSize();

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

void DevScene::TickMonsterSpawn()
{
	if (_monsterCount < DESIRED_MONSTER_COUNT)
		SpawnObjectAtRandomPos<Monster>();
}
