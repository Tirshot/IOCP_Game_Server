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
#include "Tutorial.h"
#include "Player.h"
#include "HitEffect.h"
#include "HealEffect.h"
#include "Sign.h"
#include "TutorialTrigger.h"
#include "MerchantTrigger.h"
#include "MerchantTutorialTrigger.h"
#include "UI.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "SoundManager.h"
#include "Sound.h"
#include "Merchant.h"
#include "Monster.h"
#include "MyPlayer.h"
#include "Item.h"
#include "HeartItem.h"
#include "StatusPanel.h"
#include "SceneManager.h"
#include "WeaponSlot.h"
#include "Chat.h"
#include "ChatInput.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"
#include "Arrow.h"
#include "ArrowItem.h"
#include "GameOver.h"
#include "DeathEffect.h"
#include "ShopUI.h"
#include "MerchantUI.h"
#include "QuestUI.h"
#include "Quest.h"
#include "Inventory.h"
#include "QuickSlot.h"
#include <filesystem>

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
	GET_SINGLE(ResourceManager)->LoadTexture(L"Potion", L"Sprite\\Item\\Potion.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Heart", L"Sprite\\UI\\Heart.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"HeartItem", L"Sprite\\Item\\HeartItem.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"FullHeartItem", L"Sprite\\Item\\FullHeartItem.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"MaxHeartItem", L"Sprite\\Item\\MaxHeartItem.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"SwordItem", L"Sprite\\Item\\SwordItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"BowItem", L"Sprite\\Item\\BowItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"StaffItem", L"Sprite\\Item\\StaffItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"ArrowItem", L"Sprite\\Item\\ArrowItem.bmp", RGB(255,0,255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"BlackMp", L"Sprite\\UI\\BlackMp.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"MP", L"Sprite\\UI\\MP.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Buttons", L"Sprite\\UI\\Buttons.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Teleport", L"Sprite\\Effect\\Teleport.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Death", L"Sprite\\Effect\\Death.bmp", RGB(255,0,255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Heal", L"Sprite\\Effect\\Heal.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Sign", L"Sprite\\NPC\\Sign.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Merchant", L"Sprite\\NPC\\Merchant.bmp", RGB(255, 0, 255));
	
	// UI 텍스쳐
	GET_SINGLE(ResourceManager)->LoadTexture(L"GameOver", L"Sprite\\UI\\GameOver.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Bow", L"Sprite\\Item\\Bow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Staff", L"Sprite\\Item\\Staff.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"QuickSlot", L"Sprite\\UI\\QuickSlot.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"MainFrame", L"Sprite\\UI\\MainFrame.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Selected", L"Sprite\\UI\\Selected.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Pop", L"Sprite\\UI\\pop.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"PopBackground", L"Sprite\\UI\\PopBackground.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"ShopButtonsBackground", L"Sprite\\UI\\ShopButtonsBackground.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"QuestButtonsBackground", L"Sprite\\UI\\QuestButtonsBackground.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Chat", L"Sprite\\UI\\Chat.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"ChatInput", L"Sprite\\UI\\Chat.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Gold", L"Sprite\\UI\\Gold.bmp", RGB(255,0,255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Crown", L"Sprite\\UI\\Crown.bmp", RGB(255,0,255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"ArrowUI", L"Sprite\\UI\\ArrowUI.bmp", RGB(0, 255, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Tutorial", L"Sprite\\UI\\Tutorial.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"MerchantTutorial", L"Sprite\\UI\\MerchantTutorial.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"ShopUIBackground", L"Sprite\\UI\\ShopUI.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Inventory", L"Sprite\\UI\\inventory.bmp", RGB(255, 0, 255));

	// 맵 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01"));
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileO", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 0, 0 ,48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileX", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 48, 0, 48, 48);
	
	// Item 스프라이트
	// 	GET_SINGLE(ResourceManager)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Potion", GET_SINGLE(ResourceManager)->GetTexture(L"Potion"), 0, 0, 32, 32);
	GET_SINGLE(ResourceManager)->CreateSprite(L"FullHeartItem", GET_SINGLE(ResourceManager)->GetTexture(L"FullHeartItem"), 50, 0, 25, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MaxHeartItem", GET_SINGLE(ResourceManager)->GetTexture(L"MaxHeartItem"), 0, 0, 48, 20);
	GET_SINGLE(ResourceManager)->CreateSprite(L"ArrowItem", GET_SINGLE(ResourceManager)->GetTexture(L"ArrowItem"), 0, 0, 24, 23);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SwordItem", GET_SINGLE(ResourceManager)->GetTexture(L"SwordItem"), 0, 0, 48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BowItem", GET_SINGLE(ResourceManager)->GetTexture(L"BowItem"), 0, 0, 51, 51);
	GET_SINGLE(ResourceManager)->CreateSprite(L"StaffItem", GET_SINGLE(ResourceManager)->GetTexture(L"StaffItem"), 0, 0, 39, 42);

	// UI 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"GameOver", GET_SINGLE(ResourceManager)->GetTexture(L"GameOver"), 0, 0, 400, 100);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BlackHeart", GET_SINGLE(ResourceManager)->GetTexture(L"Heart"), 0, 0, 25, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Heart", GET_SINGLE(ResourceManager)->GetTexture(L"Heart"), 25, 0, 50, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Sword", GET_SINGLE(ResourceManager)->GetTexture(L"Sword"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Bow", GET_SINGLE(ResourceManager)->GetTexture(L"Bow"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Staff", GET_SINGLE(ResourceManager)->GetTexture(L"Staff"), 12, 3, 52, 52);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Frame", GET_SINGLE(ResourceManager)->GetTexture(L"Frame"), 0, 0, 76, 76);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MainFrame", GET_SINGLE(ResourceManager)->GetTexture(L"MainFrame"), 0, 0, 462, 695);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pop", GET_SINGLE(ResourceManager)->GetTexture(L"Pop"), 0, 0, 130, 28);
	GET_SINGLE(ResourceManager)->CreateSprite(L"PopBackground", GET_SINGLE(ResourceManager)->GetTexture(L"PopBackground"), 0, 0, 300, 250);
	GET_SINGLE(ResourceManager)->CreateSprite(L"QuickSlot", GET_SINGLE(ResourceManager)->GetTexture(L"QuickSlot"), 0, 0, 480, 65);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Selected", GET_SINGLE(ResourceManager)->GetTexture(L"Selected"), 0, 0, 88, 88);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Chat", GET_SINGLE(ResourceManager)->GetTexture(L"Chat"), 0, 0, 130, 28);
	GET_SINGLE(ResourceManager)->CreateSprite(L"ChatInput", GET_SINGLE(ResourceManager)->GetTexture(L"ChatInput"), 0, 0, 400, 28);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Gold", GET_SINGLE(ResourceManager)->GetTexture(L"Gold"), 23, 24, 23, 24);
	GET_SINGLE(ResourceManager)->CreateSprite(L"ArrowUI", GET_SINGLE(ResourceManager)->GetTexture(L"ArrowUI"), 0, 0, 24, 23);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Tutorial", GET_SINGLE(ResourceManager)->GetTexture(L"Tutorial"), 0, 0, 400, 300);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MerchantTutorial", GET_SINGLE(ResourceManager)->GetTexture(L"MerchantTutorial"), 0, 0, 400, 300);
	GET_SINGLE(ResourceManager)->CreateSprite(L"LLButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 84, 540, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"LButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 184, 540, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"RButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 284, 540, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"RRButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 384, 540, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"PurchaseButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 484, 540, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"AcceptButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 84, 640, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"CompleteButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 184, 640, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"FinishedButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 284, 640, 32, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Inventory", GET_SINGLE(ResourceManager)->GetTexture(L"Inventory"), 0, 0, 290, 356);

	LoadMap();
	LoadPlayer();
	LoadMonster();
	LoadNPC();
	LoadItem();
	LoadProjectile();
	LoadEffect();
	LoadTilemap();
	LoadUI();
	LoadSound();

	Super::Init();
}

void DevScene::Update()
{
	Super::Update();
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
		SpawnObject<DeathEffect>(monster->GetCellPos());
		_monsterCount--;
		return;
	}

	// 사망시 메세지 및 UI 출력
	MyPlayer* player = dynamic_cast<MyPlayer*>(actor);
	if (player)
	{
		SpawnObject<DeathEffect>(player->GetCellPos());

		GET_SINGLE(SoundManager)->Play(L"GameOver");
		Chat* chat = GET_SINGLE(ChatManager)->GetChat();
		chat->AddText(L"캐릭터가 쓰러졌습니다.");
		for (auto& ui : _uis)
		{
			GameOver* go = dynamic_cast<GameOver*>(ui);
			if (go)
				go->SetVisible(true);
		}
	}
}

void DevScene::ClearActors()
{
	for (auto& actorLayer : _actors)
	{
		for(auto& actor : actorLayer)
			delete actor;

		actorLayer.clear();
	}
}

void DevScene::ClearUIs()
{
	for (auto& ui : _uis)
		delete ui;

	_uis.clear();
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
		fb->SetInfo({ texture, L"FB_MoveUp", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveDown", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveLeft", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MoveRight", { 200, 200}, 0, 9, 1, 0.53f });
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

	// 플레이어 스핀공격
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinReadyDown", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinDown", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinReadyUp", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinUp", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinReadyLeft", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinLeft", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinReadyRight", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SpinRight", { 200, 200}, 0, 24, 11, 0.4f, false });
	}
}

void DevScene::LoadMonster()
{
	// Snake Move
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUp");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 3, 3, 0.6f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDown");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 0, 3, 0, 0.6f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeft");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 0, 3, 2, 0.6f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRight");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 0, 3, 1, 0.6f });
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

void DevScene::LoadNPC()
{
	// 표지판
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Sign");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SignIdle");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SignIdle", { 48, 96}, 0, 0, 0, 0 });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Sign");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SignMove");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_SignMove", { 48, 96}, 1, 3, 0, 3.f });
	}
	{	// 튜토리얼 트리거 배치
		TutorialTrigger* nc = new TutorialTrigger();
		nc->info.set_posx(4);
		nc->info.set_posy(4);
		SpawnObject<TutorialTrigger>(nc->GetCellPos());
	}
	{	// 상인 튜토리얼 트리거 배치
		TutorialTrigger* nc = new TutorialTrigger();
		nc->info.set_posx(28);
		nc->info.set_posy(5);
		SpawnObject<MerchantTutorialTrigger>(nc->GetCellPos());
	}
	// 상인
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Merchant");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MerchantIdle");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MerchantIdle", { 60, 100}, 1, 3, 0, 3.f });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Merchant");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MerchantMove");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_MerchantMove", { 60, 100}, 0, 0, 0, 0.f });
	}
	{	// 상점 트리거 배치
		MerchantTrigger* mt = new MerchantTrigger();
		mt->info.set_posx(40);
		mt->info.set_posy(26);
		SpawnObject<MerchantTrigger>(mt->GetCellPos());
	}
}

void DevScene::LoadItem()
{
	{ // Heart 아이템
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"HeartItem");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_HeartItem");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"HeartItem", { 25, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Full Heart 아이템
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"FullHeartItem");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_FullHeartItem");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FullHeartItem", { 25, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Max Heart 아이템
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"MaxHeartItem");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MaxHeartItem");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"MaxHeartItem", { 50, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Arrow 아이템
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"ArrowItem");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowItem");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"ArrowItem", { 24, 23}, 0, 4, 0, 0.8f, true });
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
	// Teleport Effect
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Teleport");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Teleport");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_Teleport", { 80, 70}, 0, 9, 0, 0.3f, false });
	}
	// Death Effect
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Death");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Death");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_Death", { 80, 80}, 0, 5, 0, 0.5f, false });
	}
	// Heal Effect
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Heal");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Heal");
		// SetInfo({텍스쳐, 이름, {한 개의 사이즈}, 시작, 끝, 줄, 시간});
		fb->SetInfo({ texture, L"FB_Heal", { 80, 70}, 0, 4, 0, 0.2f, false });
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
	//{	// 무기 슬롯, pivot은 중앙 하단
	//	WeaponSlot* ws = new WeaponSlot();
	//	if (ws)
	//	{
	//		ws->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
	//		
	//		ws->SetVisible(true);
	//		AddUI(ws);
	//	}
	//}
	{	// 퀵 슬롯
		QuickSlot* qs = GET_SINGLE(QuickSlot);
		if (qs)
		{
			qs->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY });
			qs->SetVisible(true);
			AddUI(qs);
		}

	}
	{  // 채팅 창
		Chat* chat = new Chat(Vec2{ 10,320 } /*Texts*/);
		if (chat)
		{// background
			chat->SetPos(Vec2{ 10,320 });
			chat->SetVisible(true);
			chat->SetAlpha(250);
			GET_SINGLE(ChatManager)->SetChat(chat);
			AddUI(chat);
		}
	}
	{	// Chatting input
		ChatInput* chatInput = new ChatInput();
		if (chatInput)
		{
			/*chatInput->SetSize(Vec2Int{ 300,20 });*/
			GET_SINGLE(ChatManager)->SetChatInput(chatInput);
			AddUI(chatInput);
		}
	}
	{
		StatusPanel* statusPanel = new StatusPanel();
		if (statusPanel)
		{
			statusPanel->SetVisible(true);
			AddUI(statusPanel);
		}
	}
	{	// Game Over
		GameOver* go = new GameOver();
		if (go)
		{
			go->SetVisible(false);
			AddUI(go);
		}
	}
	{	// 상인 UI
		MerchantUI* tu = new MerchantUI();
		if (tu)
		{
			tu->SetVisible(false);
			tu->SetPos({ 300,50 });
			AddUI(tu);
		}
	}
	{	// 상인 - 상점 UI
		ShopUI* shopUI = new ShopUI();
		if (shopUI)
		{
			shopUI->SetPos({ 135,80 });
			shopUI->SetSize({ 535,450 });
			shopUI->SetVisible(false);
			AddUI(shopUI);
		}
	}
	{	// 상인 - 퀘스트 UI
		QuestUI* questUI = new QuestUI();
		if (questUI)
		{
			questUI->SetPos({ 135,80 });
			questUI->SetSize({ 535,450 });
			questUI->SetVisible(false);
			AddUI(questUI);
		}
	}
	{	// 인벤토리 및 장비창
		Inventory* Inven = new Inventory();
		if (Inven)
		{
			Inven->SetPos({ 480, 125 });
			Inven->SetVisible(false);
			AddUI(Inven);
		}
	}
}

void DevScene::LoadSound()
{
	/*GET_SINGLE(ResourceManager)->LoadSound(L"BGM", L"Sound\\BGM.wav");
	GET_SINGLE(SoundManager)->Play(L"BGM", true);*/
	GET_SINGLE(ResourceManager)->LoadSound(L"Land", L"Sound\\Land.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Button", L"Sound\\Button.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Sword", L"Sound\\Sword.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Bow", L"Sound\\Bow.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Staff", L"Sound\\Staff.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Potion", L"Sound\\Potion.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Heart", L"Sound\\Heart.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Teleport", L"Sound\\Teleport.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"SpinAttack", L"Sound\\SpinAttack.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"SpinReady", L"Sound\\SpinReady.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestAccept", L"Sound\\QuestAccept.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestComplete", L"Sound\\QuestComplete.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestFinished", L"Sound\\QuestFinished.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"MonsterOnDamaged", L"Sound\\MonsterOnDamaged.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"PlayerOnDamaged", L"Sound\\PlayerOnDamaged.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"GameOver", L"Sound\\GameOver.wav");
	GET_SINGLE(ResourceManager)->LoadSound(L"Merchant", L"Sound\\Merchant.wav");
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
		else if (info.objecttype() == Protocol::OBJECT_TYPE_NPC)
		{
			switch (info.npctype())
			{
			case Protocol::NPC_TYPE_SIGN:
			{
				Sign* npc = SpawnObject<Sign>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				npc->info = info;
				npc->SetDir(DIR_DOWN);
				npc->SetState(info.state());
			}
				break;

			case Protocol::NPC_TYPE_MERCHANT:
			{
				Merchant* npc = SpawnObject<Merchant>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				npc->info = info;
				npc->SetDir(DIR_DOWN);
				npc->SetState(info.state());
			}
				break;
			}
		}
		else if (info.objecttype() == Protocol::OBJECT_TYPE_ITEM)
		{
			switch (info.itemtype())
			{
			case Protocol::ITEM_TYPE_HEART:
			{
				HeartItem* item = SpawnObject<HeartItem>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				item->info = info;
				item->SetState(IDLE);
			}
				break;

			case Protocol::ITEM_TYPE_FULLHEART:
			{
				HeartItem* item = SpawnObject<HeartItem>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				item->info = info;
				item->SetState(IDLE);
			}
				break;

			case Protocol::ITEM_TYPE_MAXHEART:
			{
				HeartItem* item = SpawnObject<HeartItem>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				item->info = info;
				item->SetState(IDLE);
			}
			break;

			case Protocol::ITEM_TYPE_ARROW:
			{
				ArrowItem* item = SpawnObject<ArrowItem>(Vec2Int{ info.posx(), info.posy() });
				// 애니메이션을 위해
				item->info = info;
				item->SetState(IDLE);
			}
			break;

			default:
				return;
			}

		}
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

	// 1이면 갈 수 없는 지형
	return tile->value != 1;
}

bool DevScene::MonsterCanGo(Vec2Int cellPos)
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

	if (tile->value == 2)
		return false;

	// 1이면 갈 수 없는 지형
	return tile->value == 0;
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

void DevScene::SetPlayerQuestState(int playerId, int questId, Protocol::QUEST_STATE state)
{
	MyPlayer* player = GET_SINGLE(SceneManager)->GetMyPlayer();
	player->SetQuestState(questId, state, 0);
}

Protocol::QUEST_STATE DevScene::GetPlayerQuestState(int objectId, int questId)
{
	MyPlayer* player = GET_SINGLE(SceneManager)->GetMyPlayer();
	if (player)
		return player->GetQuestState(questId);
}

void DevScene::TickMonsterSpawn()
{
	// 더 이상 클라이언트에서 몬스터를 스폰하지 않음.
	return;

	if (_monsterCount < DESIRED_MONSTER_COUNT)
		SpawnObjectAtRandomPos<Monster>();
}
