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
#include "MyPlayer.h"
#include "SceneManager.h"
#include "WeaponSlot.h"
#include "Chat.h"
#include "ChatManager.h"
#include "Arrow.h"
#include "HP.h"

DevScene::DevScene()
{

}

DevScene::~DevScene()
{

}

void DevScene::Init()
{
	// 모든 리소스는 Scene에서 불러온다
	// 다른 Scene으로 넘어갈 때 제거(Clear)한다
	// 스테이지 01
	GET_SINGLE(ResourceManager)->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");

	// 타일, 화살, 크리쳐의 텍스쳐
	GET_SINGLE(ResourceManager)->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Potion", L"Sprite\\UI\\Mp.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Heart", L"Sprite\\UI\\Heart.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0,0,0));
	
	// UI 텍스쳐
	GET_SINGLE(ResourceManager)->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Bow", L"Sprite\\Item\\Bow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Staff", L"Sprite\\Item\\Staff.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Frame", L"Sprite\\UI\\Frame.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"MainFrame", L"Sprite\\UI\\MainFrame.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Slot", L"Sprite\\UI\\Slot.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"SelectedSlot", L"Sprite\\UI\\SelectedSlot.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Selected", L"Sprite\\UI\\Selected.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Chat", L"Sprite\\UI\\Chat.bmp");

	// 맵 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01"));
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileO", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 0, 0 ,48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileX", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 48, 0, 48, 48);
	
	// UI 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Start"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_On", GET_SINGLE(ResourceManager)->GetTexture(L"Start"), 150, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Edit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Edit"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Edit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Edit"), 150, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Exit"), 0, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Exit"), 150, 0, 150, 150);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BlackHeart", GET_SINGLE(ResourceManager)->GetTexture(L"Heart"), 0, 0, 25, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Heart", GET_SINGLE(ResourceManager)->GetTexture(L"Heart"), 25, 0, 50, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Sword", GET_SINGLE(ResourceManager)->GetTexture(L"Sword"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Bow", GET_SINGLE(ResourceManager)->GetTexture(L"Bow"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Staff", GET_SINGLE(ResourceManager)->GetTexture(L"Staff"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Frame", GET_SINGLE(ResourceManager)->GetTexture(L"Frame"), 0, 0, 76, 76);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MainFrame", GET_SINGLE(ResourceManager)->GetTexture(L"MainFrame"), 0, 0, 462, 695);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Slot", GET_SINGLE(ResourceManager)->GetTexture(L"Slot"), 0, 0, 76, 76);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SelectedSlot", GET_SINGLE(ResourceManager)->GetTexture(L"SelectedSlot"), 0, 0, 44, 44);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Selected", GET_SINGLE(ResourceManager)->GetTexture(L"Selected"), 0, 0, 88, 88);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Chat", GET_SINGLE(ResourceManager)->GetTexture(L"Chat"), 0, 0, 130, 28);


	LoadMap();
	LoadPlayer();
	LoadMonster();
	LoadProjectile();
	LoadEffect();
	LoadTilemap();
	LoadUI();

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

	// 사망시 메세지 출력
	MyPlayer* player = dynamic_cast<MyPlayer*>(actor);
	if (player)
	{
		Chat* chat = GET_SINGLE(ChatManager)->GetChat();
		chat->AddText(L"캐릭터가 쓰러졌습니다.");
	}
}

void DevScene::LoadMap()
{
	// 배경
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
	// 플레이어 Idle
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_IdleUp", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_IdleDown", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_IdleLeft", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_IdleRight", { 200, 200}, 0, 9, 0, 0.5f });
	}
	// 플레이어 Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveUp", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveDown", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveLeft", { 200, 200}, 0, 9, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveRight", { 200, 200}, 0, 9, 1, 0.5f });
	}
	// 플레이어 Attack, 매개변수 끝은 loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_AttackUp", { 200, 200}, 0, 7, 3, 0.3f , false});
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_AttackDown", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_AttackLeft", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_AttackRight", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	// 플레이어 Bow, 매개변수 끝은 loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_BowUp", { 200, 200}, 0, 5, 5, 0.3f , false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_BowDown", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_BowLeft", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_BowRight", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	// 플레이어 Staff, 매개변수 끝은 loop
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_StaffUp", { 200, 200}, 0, 10, 6, 0.5f , false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_StaffDown", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_StaffLeft", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_StaffRight", { 200, 200}, 0, 10, 6, 0.5f, false });
	}


}

void DevScene::LoadMonster()
{
	// Snake Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 3, 3, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 0, 3, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 0, 3, 2, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 0, 3, 1, 0.5f });
	}

	// Snake Hit
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUpHit");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 0, 5, 1 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDownHit");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 3, 3, 5, 1 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeftHit");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 1, 1, 5, 1 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRightHit");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 2, 2, 5, 1 });
	}

}

void DevScene::LoadProjectile()
{
	// Arrow Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_ArrowUp", { 100, 100}, 0, 0, 3, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_ArrowDown", { 100, 100}, 0, 0, 0, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_ArrowLeft", { 100, 100}, 0, 0, 1, 0.5f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_ArrowRight", { 100, 100}, 0, 0, 2, 0.5f });
	}
}

void DevScene::LoadEffect()
{
	// HitEffect
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Hit");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Hit");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_Hit", { 50, 47}, 0, 5, 0, 0.3f, false });
	}
}

void DevScene::LoadTilemap()
{
	// 타일맵
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

void DevScene::LoadUI()
{
	{	// 무기 슬롯, pivot은 중앙 하단
		WeaponSlot* ws = new WeaponSlot();
		AddUI(ws);
		ws->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY });
		ws->SetVisible(true);
	}
	{  // 채팅 창
		Chat* chat = new Chat(Vec2{ 10,320 });
		GET_SINGLE(ChatManager)->SetChat(chat);
		AddUI(chat);
		
		chat->SetPos(Vec2{ 10,320 });
	}
	{ // HP
		HP* hp = new HP();
		AddUI(hp);
		hp->SetVisible(true);
	}
}

GameObject* DevScene::GetObjects(uint64 id)
{
	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(actor);
		if (gameObject && gameObject->info.objectid() == id)
			return gameObject;
	}

	return nullptr;
}

void DevScene::Handle_S_AddObject(Protocol::S_AddObject& pkt)
{
	// 자신에 대한 정보가 이미 있다면 스킵
	uint64 myplayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
	
	// repeated 패킷을 순회
	const int32 size =pkt.objects_size();
	for (int32 i = 0; i < size; i++)
	{
		const Protocol::ObjectInfo& info = pkt.objects(i);
		if (myplayerId == info.objectid())
			continue;

		if (info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
		{
			Player* player = SpawnObject<Player>(Vec2Int{ info.posx(), info.posy() });
			
			// 애니메이션을 위해
			player->info = info;
			player->SetDir(info.dir());
			player->SetState(info.state());
		}
		else if (info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
		{
			Monster* monster = SpawnObject<Monster>(Vec2Int{ info.posx(), info.posy() });

			// 애니메이션을 위해
			monster->info = info;
			monster->SetDir(info.dir());
			monster->SetState(info.state());
		}
		//else if (info.objecttype() == Protocol::OBJECT_TYPE_PROJECTILE)
		//{
		//	Arrow* arrow = SpawnObject<Arrow>(Vec2Int{ info.posx(), info.posy() });

		//	// 애니메이션을 위해
		//	arrow->info = info;
		//	arrow->SetDir(info.dir());
		//	arrow->SetState(info.state());
		//}
	}
}

void DevScene::Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt)
{
	const int32 size = pkt.ids_size();
	for (int32 i = 0; i < size; i++)
	{
		uint64 id = pkt.ids(i);

		GameObject* object = GetObjects(id);
		if (object)
			RemoveActor(object);
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
	// 맵 끝까지 찾지 못하도록 예외처리
	int32 depth = abs(dest.y - src.y) + abs(dest.x - src.x);
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
				bestScore = (float)score;
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

	// 몬스터 충돌
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

	ret.x = pos.x + cellPos.x * size + (size / static_cast<float>(2));
	ret.y = pos.y + cellPos.y * size + (size / static_cast<float>(2));

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

void DevScene::TickMonsterSpawn()
{
	// 더 이상 클라이언트에서 몬스터를 스폰하지 않음.
	return;

	if (_monsterCount < DESIRED_MONSTER_COUNT)
		SpawnObjectAtRandomPos<Monster>();
}
