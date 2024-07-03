#include "pch.h"
#include "DevScene.h"
#include "Utils.h"
#include "ItemManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "SpriteActor.h"
#include "Flipbook.h"
#include "Tutorial.h"
#include "Player.h"
#include "Item.h"
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
#include "Snake.h"
#include "Moblin.h"
#include "Octoroc.h"
#include "MyPlayer.h"
#include "Item.h"
#include "StatusPanel.h"
#include "SceneManager.h"
#include "Chat.h"
#include "ChatInput.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "QuestManager.h"
#include "ClientPacketHandler.h"
#include "Arrow.h"
#include "GameOver.h"
#include "DeathEffect.h"
#include "ShopUI.h"
#include "MerchantUI.h"
#include "QuestUI.h"
#include "Quest.h"
#include "QuestTrackerUI.h"
#include "Inventory.h"
#include "QuickSlot.h"
#include "AlertBox.h"
#include "SettingPanel.h"
#include "MerchantDialogueUI.h"
#include "MiniMap.h"
#include <filesystem>

DevScene::DevScene()
{

}

DevScene::~DevScene()
{
	_quests.clear();
}

void DevScene::Init()
{
	// ��� ���ҽ��� Scene���� �ҷ��´�
	// �ٸ� Scene���� �Ѿ �� ����(Clear)�Ѵ�
	// �������� 01
	GET_SINGLE(ResourceManager)->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Stage01_mini", L"Sprite\\Map\\Stage01_mini.bmp");

	// Ÿ��, ȭ��, ũ������ �ؽ���
	GET_SINGLE(ResourceManager)->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Potion", L"Sprite\\Item\\Potion.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Heart", L"Sprite\\UI\\Heart.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Buttons", L"Sprite\\UI\\Buttons.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Moblin", L"Sprite\\Monster\\Moblin.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Octoroc", L"Sprite\\Monster\\Octoroc.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Teleport", L"Sprite\\Effect\\Teleport.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Death", L"Sprite\\Effect\\Death.bmp", RGB(255,0,255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Heal", L"Sprite\\Effect\\Heal.bmp", RGB(0,0,0));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Sign", L"Sprite\\NPC\\Sign.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Merchant", L"Sprite\\NPC\\Merchant.bmp", RGB(255, 0, 255));
	
	// ������ �ؽ���
	GET_SINGLE(ResourceManager)->LoadTexture(L"HeartItem", L"Sprite\\Item\\HeartItem.bmp", RGB(128,128,128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"FullHeartItem", L"Sprite\\Item\\FullHeartItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"MaxHeartItem", L"Sprite\\Item\\MaxHeartItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"SwordItem", L"Sprite\\Item\\SwordItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"BowItem", L"Sprite\\Item\\BowItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"StaffItem", L"Sprite\\Item\\StaffItem.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"ArrowItem", L"Sprite\\Item\\ArrowItem.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"EquipItem", L"Sprite\\Item\\Equips.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"SnakeSkin", L"Sprite\\Item\\SnakeSkin.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"MoblinSpear", L"Sprite\\Item\\MoblinSpear.bmp", RGB(128, 128, 128));

	// UI �ؽ���
	GET_SINGLE(ResourceManager)->LoadTexture(L"BlackMp", L"Sprite\\UI\\BlackMp.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"MP", L"Sprite\\UI\\MP.bmp");
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
	GET_SINGLE(ResourceManager)->LoadTexture(L"Icon", L"Sprite\\UI\\Icon.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Settings", L"Sprite\\UI\\Settings.bmp", RGB(128, 128, 128));

	// �� ��������Ʈ
	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01"));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01_mini", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01_mini"));
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileO", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 0, 0 ,48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"TileX", GET_SINGLE(ResourceManager)->GetTexture(L"Tile"), 48, 0, 48, 48);
	
	// Item ��������Ʈ
	// 	GET_SINGLE(ResourceManager)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Potion", GET_SINGLE(ResourceManager)->GetTexture(L"Potion"), 0, 0, 32, 32);
	GET_SINGLE(ResourceManager)->CreateSprite(L"HeartItem", GET_SINGLE(ResourceManager)->GetTexture(L"HeartItem"), 0, 0, 25, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"FullHeartItem", GET_SINGLE(ResourceManager)->GetTexture(L"FullHeartItem"), 50, 0, 25, 21);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MaxHeartItem", GET_SINGLE(ResourceManager)->GetTexture(L"MaxHeartItem"), 0, 0, 48, 20);
	GET_SINGLE(ResourceManager)->CreateSprite(L"ArrowItem", GET_SINGLE(ResourceManager)->GetTexture(L"ArrowItem"), 0, 0, 24, 23);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SwordItem", GET_SINGLE(ResourceManager)->GetTexture(L"SwordItem"), 0, 0, 48, 48);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BowItem", GET_SINGLE(ResourceManager)->GetTexture(L"BowItem"), 0, 0, 51, 51);
	GET_SINGLE(ResourceManager)->CreateSprite(L"StaffItem", GET_SINGLE(ResourceManager)->GetTexture(L"StaffItem"), 0, 0, 39, 42);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Helmet00", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 0, 0, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Helmet01", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 42, 0, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Helmet02", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 83, 0, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Helmet03", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 127, 0, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Armor00", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 0, 30, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Armor01", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 42, 30, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Armor02", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 83, 30, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Armor03", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 127, 30, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pants00", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 0, 60, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pants01", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 42, 60, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pants02", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 83, 60, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pants03", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 127, 60, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Boots00", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 0, 90, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Boots01", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 42, 90, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Boots02", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 83, 90, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Boots03", GET_SINGLE(ResourceManager)->GetTexture(L"EquipItem"), 127, 90, 30, 30);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SnakeSkin", GET_SINGLE(ResourceManager)->GetTexture(L"SnakeSkin"), 0, 0, 64, 62);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MoblinSpear", GET_SINGLE(ResourceManager)->GetTexture(L"MoblinSpear"), 0, 0, 64, 64);


	// UI ��������Ʈ
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Revive_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 30, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Back_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Confirm_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 630, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Confirm_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 830, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Confirm_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 1030, 230, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Shop_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Accept_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 630, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Accept_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 830, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Accept_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 1030, 330, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Decline_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 630, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Decline_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 830, 430, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Quest_Decline_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 1030, 430, 140, 40);
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
	GET_SINGLE(ResourceManager)->CreateSprite(L"PopAcceptButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 615, 30, 60, 35);
	GET_SINGLE(ResourceManager)->CreateSprite(L"PopDenyButton", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 615, 130, 60, 35);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Inventory", GET_SINGLE(ResourceManager)->GetTexture(L"Inventory"), 0, 0, 290, 356);
	GET_SINGLE(ResourceManager)->CreateSprite(L"DangerIcon", GET_SINGLE(ResourceManager)->GetTexture(L"Icon"), 0, 0, 34, 34);
	GET_SINGLE(ResourceManager)->CreateSprite(L"AlertIcon", GET_SINGLE(ResourceManager)->GetTexture(L"Icon"), 34, 0, 34, 34);
	GET_SINGLE(ResourceManager)->CreateSprite(L"InformationIcon", GET_SINGLE(ResourceManager)->GetTexture(L"Icon"), 68, 0, 34, 34);
	GET_SINGLE(ResourceManager)->CreateSprite(L"MerchantSprite", GET_SINGLE(ResourceManager)->GetTexture(L"Merchant"), 0, 0, 60, 100);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SoundSettings", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 50, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BGM", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 75, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SFX", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 100, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"UI", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 125, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SliderBackground", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 145, 50, 143, 12);

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
	LoadQuest();

	Super::Init();
}

void DevScene::Update()
{
	Super::Update();

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::ESC))
	{
		if (IsAnyPopUpVisible() == false)
		{
			auto setting = FindUI<SettingPanel>(_uis);
			if (setting)
				setting->SetVisible(true);
		}
	}
}

void DevScene::Render(HDC hdc)
{
	Super::Render(hdc);

	// ���̵� ��
	if (_alpha >= 10)
	{
		BLENDFUNCTION bf = {};
		bf.AlphaFormat = 0; //�Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER; //  ������ ��� �̹����� ��ħ
		bf.SourceConstantAlpha = _alpha; // ����(���� 0 - ������ 255)

		::AlphaBlend(hdc,
			0,
			0,
			800,
			600,
			_black->GetDC(),
			0,
			0,
			100,
			100,
			bf);

		_alpha -= 1;
	}
}

void DevScene::AddActor(shared_ptr<Actor> actor)
{
	Super::AddActor(actor);

	shared_ptr<Monster> monster = dynamic_pointer_cast<Monster>(actor);
	if (monster)
	{
		_monsterCount++;
	}
}

void DevScene::RemoveActor(shared_ptr<Actor> actor)
{
	Super::RemoveActor(actor);

	shared_ptr<Monster> monster = dynamic_pointer_cast<Monster>(actor);
	if (monster)
	{
		SpawnObject<DeathEffect>(monster->GetCellPos());

		GET_SINGLE(SoundManager)->Play(L"MonsterOnDamaged");

		_monsterCount--;
		return;
	}

	// ����� �޼��� �� UI ���
	shared_ptr<MyPlayer> player = dynamic_pointer_cast<MyPlayer>(actor);
	if (player)
	{
		SpawnObject<DeathEffect>(player->GetCellPos());

		GET_SINGLE(SoundManager)->Play(L"GameOver");

		shared_ptr<Chat> chat = GET_SINGLE(ChatManager)->GetChat();
		chat->AddText(L"ĳ���Ͱ� ���������ϴ�.");

		for (auto& ui : _uis)
		{
			shared_ptr<GameOver> go = dynamic_pointer_cast<GameOver>(ui);
			if (go)
				go->SetVisible(true);
		}
	}
}

void DevScene::ClearActors()
{
	for (auto& actorLayer : _actors)
	{
		actorLayer.clear();
	}
}

void DevScene::ClearUIs()
{
	_uis.clear();
}

void DevScene::LoadMap()
{
	// ���
	auto sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Stage01");

	shared_ptr<SpriteActor> background = make_shared<SpriteActor>();
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
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_IdleUp", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_IdleDown", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_IdleLeft", { 200, 200}, 0, 9, 0, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_IdleRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_IdleRight", { 200, 200}, 0, 9, 0, 0.5f });
	}
	// �÷��̾� Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveUp", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveDown", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveLeft", { 200, 200}, 0, 9, 1, 0.53f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoveRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoveRight", { 200, 200}, 0, 9, 1, 0.53f });
	}
	// �÷��̾� Attack, �Ű����� ���� loop
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackUp", { 200, 200}, 0, 7, 3, 0.3f , false});
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackDown", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackLeft", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_AttackRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_AttackRight", { 200, 200}, 0, 7, 3, 0.3f, false });
	}
	// �÷��̾� Bow, �Ű����� ���� loop
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowUp", { 200, 200}, 0, 5, 5, 0.3f , false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowDown", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowLeft", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_BowRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_BowRight", { 200, 200}, 0, 5, 5, 0.3f, false });
	}
	// �÷��̾� Staff, �Ű����� ���� loop
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffUp", { 200, 200}, 0, 10, 6, 0.5f , false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffDown", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffLeft", { 200, 200}, 0, 10, 6, 0.5f, false });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_StaffRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_StaffRight", { 200, 200}, 0, 10, 6, 0.5f, false });
	}

	// �÷��̾� ���ɰ���
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinReadyDown", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDown");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinDown", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinReadyUp", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerUp");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinUp", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinReadyLeft", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLeft");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinLeft", { 200, 200}, 0, 22, 11, 0.4f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinReadyRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinReadyRight", { 200, 200}, 9, 15, 3, 0.2f, false });
	}

	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRight");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SpinRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SpinRight", { 200, 200}, 0, 24, 11, 0.4f, false });
	}
}

void DevScene::LoadMonster()
{	
	// Snake Idle
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeIdleUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeIdleUp", { 100, 100}, 0, 0, 3, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeIdleDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeIdleDown", { 100, 100}, 0, 0, 0, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeIdleLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeIdleLeft", { 100, 100}, 0, 0, 2, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeIdleRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeIdleRight", { 100, 100}, 0, 0, 1, 0.6f, true });
	}
	// Snake Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 3, 3, 0.6f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 0, 3, 0, 0.6f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 0, 3, 2, 0.6f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 0, 3, 1, 0.6f });
	}

	// Snake Hit
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeUpHit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeUp", { 100, 100}, 0, 0, 5, 1 });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeDownHit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeDown", { 100, 100}, 3, 3, 5, 1 });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeLeftHit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeLeft", { 100, 100}, 1, 1, 5, 1 });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Snake");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SnakeRightHit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SnakeRight", { 100, 100}, 2, 2, 5, 1 });
	}

	// Moblin Idle
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinUp", { 300, 300}, 0, 0, 6, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinDown", { 300, 300}, 0, 0, 0, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinLeft", { 300, 300}, 0, 0, 2, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinRight", { 300, 300}, 0, 0, 4, 0.6f, true });
	}

	// Moblin Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinMoveUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinMoveUp", { 300, 300}, 0, 3, 6, 0.4f});
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinMoveDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinMoveDown", { 300, 300}, 0, 3, 0, 0.4f});
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinMoveLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinMoveLeft", { 300, 300}, 0, 3, 2, 0.4f});
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinMoveRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinMoveRight", { 300, 300}, 0, 3, 4, 0.4f});
	}

	// Moblin Attack
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinAttackUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinAttackUp", { 300, 300}, 0, 1, 6, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinAttackDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinAttackDown", { 300, 300}, 0, 1, 1, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinAttackLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinAttackLeft", { 300, 300}, 0, 1, 3, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinAttackRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinAttackRight", { 300, 300}, 0, 1, 5, 0.2f });
	}

	// Moblin Hit
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinHitUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinHitUp", { 300, 300}, 0, 0, 10, 1.f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinHitDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinHitDown", { 300, 300}, 0, 0, 7, 1.f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinHitLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinHitLeft", { 300, 300}, 0, 0, 8, 1.f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Moblin");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MoblinHitRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MoblinHitRight", { 300, 300}, 0, 0, 9, 1.f, true });
	}
	// Octoroc Idle
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocIdleUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocIdleUp", { 100, 100}, 2, 2, 6, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocIdleDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocIdleDown", { 100, 100}, 2, 2, 0, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocIdleLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocIdleLeft", { 100, 100}, 1, 1, 2, 0.6f, true });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocIdleRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocIdleRight", { 100, 100}, 1, 1, 4, 0.6f, true });
	}

	// Octoroc Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocMoveUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocMoveUp", { 100, 100}, 0, 1, 6, 0.4f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocMoveDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocMoveDown", { 100, 100}, 0, 1, 0, 0.4f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocMoveLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocMoveLeft", { 100, 100}, 0, 1, 2, 0.4f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocMoveRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocMoveRight", { 100, 100}, 0, 1, 4, 0.4f });
	}

	// Octoroc Attack
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocAttackUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocAttackUp", { 100, 100}, 0, 1, 7, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocAttackDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocAttackDown", { 100, 100}, 0, 1, 1, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocAttackLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocAttackLeft", { 100, 100}, 0, 1, 3, 0.2f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_OctorocAttackRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_OctorocAttackRight", { 100, 100}, 0, 1, 5, 0.2f });
	}
}

void DevScene::LoadNPC()
{
	// ǥ����
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Sign");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SignIdle");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SignIdle", { 48, 96}, 0, 0, 0, 0 });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Sign");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_SignMove");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_SignMove", { 48, 96}, 1, 3, 0, 3.f });
	}
	{	// Ʃ�丮�� Ʈ���� ��ġ
		auto nc = make_shared<TutorialTrigger>();
		nc->info.set_posx(4);
		nc->info.set_posy(4);
		SpawnObject<TutorialTrigger>(nc->GetCellPos());
	}
	{	// ���� Ʃ�丮�� Ʈ���� ��ġ
		auto nc = make_shared<TutorialTrigger>();
		nc->info.set_posx(28);
		nc->info.set_posy(5);
		SpawnObject<MerchantTutorialTrigger>(nc->GetCellPos());
	}
	// ����
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Merchant");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MerchantIdle");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MerchantIdle", { 60, 100}, 1, 3, 0, 3.f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Merchant");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MerchantMove");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_MerchantMove", { 60, 100}, 0, 0, 0, 0.f });
	}
	{	// ���� Ʈ���� ��ġ
		auto mt = make_shared<MerchantTrigger>();
		mt->info.set_posx(40);
		mt->info.set_posy(26);
		SpawnObject<MerchantTrigger>(mt->GetCellPos());
	}
}

void DevScene::LoadItem()
{
	{ // Heart ������
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"HeartItem");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_HeartItem");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"HeartItem", { 25, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Full Heart ������
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"FullHeartItem");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_FullHeartItem");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FullHeartItem", { 25, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Max Heart ������
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"MaxHeartItem");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_MaxHeartItem");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"MaxHeartItem", { 50, 21}, 0, 4, 0, 0.8f, true });
	}
	{ // Arrow ������
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"ArrowItem");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowItem");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"ArrowItem", { 24, 23}, 0, 4, 0, 0.8f, true });
	}
}

void DevScene::LoadProjectile()
{
	// Arrow Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowUp", { 100, 100}, 0, 0, 3, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowDown", { 100, 100}, 0, 0, 0, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowLeft", { 100, 100}, 0, 0, 1, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Arrow");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ArrowRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ArrowRight", { 100, 100}, 0, 0, 2, 0.5f });
	}
	// Rock Move
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ProjectileRockUp");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ProjectileRockUp", { 100, 100}, 0, 2, 9, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ProjectileRockDown");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ProjectileRockDown", { 100, 100}, 0, 2, 9, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ProjectileRockLeft");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ProjectileRockLeft", { 100, 100}, 0, 2, 9, 0.5f });
	}
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Octoroc");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_ProjectileRockRight");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_ProjectileRockRight", { 100, 100}, 0, 2, 9, 0.5f });
	}
}

void DevScene::LoadEffect()
{
	// HitEffect
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Hit");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Hit");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_Hit", { 50, 47}, 0, 5, 0, 0.3f, false });
	}
	// Teleport Effect
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Teleport");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Teleport");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_Teleport", { 80, 70}, 0, 9, 0, 0.3f, false });
	}
	// Death Effect
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Death");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Death");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_Death", { 80, 80}, 0, 5, 0, 0.5f, false });
	}
	// Heal Effect
	{
		shared_ptr<Texture> texture = GET_SINGLE(ResourceManager)->GetTexture(L"Heal");
		shared_ptr<Flipbook> fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"FB_Heal");
		// SetInfo({�ؽ���, �̸�, {�� ���� ������}, ����, ��, ��, �ð�});
		fb->SetInfo({ texture, L"FB_Heal", { 80, 70}, 0, 4, 0, 0.2f, false });
	}
}

void DevScene::LoadTilemap()
{
	// Ÿ�ϸ�
		shared_ptr<TilemapActor> actor = make_shared<TilemapActor>();
		AddActor(actor);

		_tilemapActor = actor;
		{
			auto tm = GET_SINGLE(ResourceManager)->CreateTilemap(L"Tilemap_01");
			tm->SetMapSize({ 63,43 });
			tm->SetTileSize(48);

			GET_SINGLE(ResourceManager)->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");
			_tilemapActor->SetTilemap(tm);
			_tilemapActor->SetShowDebug(false);
		}
}

void DevScene::LoadUI()
{
	_black = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");
	{	// �� ����
		shared_ptr<QuickSlot> qs = make_shared<QuickSlot>();
		if (qs)
		{
			qs->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY });
			qs->SetVisible(true);
			AddUI(qs);
		}
	}
	{  // ä�� â
		shared_ptr<Chat> chat = make_shared<Chat>(Vec2{ 10,320 } /*Texts*/);
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
		shared_ptr<ChatInput> chatInput = make_shared<ChatInput>();
		if (chatInput)
		{
			/*chatInput->SetSize(Vec2Int{ 300,20 });*/
			GET_SINGLE(ChatManager)->SetChatInput(chatInput);
			AddUI(chatInput);
		}
	}
	{
		shared_ptr<StatusPanel> statusPanel = make_shared<StatusPanel>();
		if (statusPanel)
		{
			statusPanel->SetVisible(true);
			AddUI(statusPanel);
		}
	}
	{	// ����Ʈ Ʈ��Ŀ
		shared_ptr<QuestTrackerUI> tracker = make_shared<QuestTrackerUI>();
		if (tracker)
		{
			tracker->SetVisible(true);
			AddUI(tracker);
		}
	}
	{	// �̴ϸ�
		auto miniMap = make_shared<MiniMap>();
		if (miniMap)
		{
			miniMap->SetVisible(true);
			miniMap->SetSize({ 120, 120 });
			miniMap->SetPos({ (float)GWinSizeX - miniMap->GetSize().x, 0});
			AddUI(miniMap);
		}
	}
	{	// Game Over
		shared_ptr<GameOver> go = make_shared<GameOver>();
		if (go)
		{
			go->SetVisible(false);
			AddUI(go);
		}
	}
	{	// ���� UI
		shared_ptr<MerchantUI> tu = make_shared<MerchantUI>();
		if (tu)
		{
			tu->SetVisible(false);
			tu->SetPos({ 300,50 });
			AddUI(tu);
		}
	}
	{
		// ���� ��ȭ â
		shared_ptr<MerchantDialogueUI> talk = make_shared<MerchantDialogueUI>();
		if (talk)
		{
			talk->SetDialogue(0);
			talk->SetVisible(false);
			AddUI(talk);
		}
	}
	{	// ���� - ���� UI
		shared_ptr<ShopUI> shopUI = make_shared<ShopUI>();
		if (shopUI)
		{
			shopUI->SetPos({ 135,80 });
			shopUI->SetSize({ 535,450 });
			shopUI->SetVisible(false);
			AddUI(shopUI);
		}
	}
	{	// ���� - ����Ʈ UI
		shared_ptr<QuestUI> questUI = make_shared<QuestUI>();
		if (questUI)
		{
			questUI->SetPos({ 135,80 });
			questUI->SetInitialPos({ 135,80 });
			questUI->SetSize({ 535,450 });
			questUI->SetVisible(false);
			AddUI(questUI);
		}
	}
	{	// �κ��丮 �� ���â
		shared_ptr<Inventory> Inven = make_shared<Inventory>();
		if (Inven)
		{
			Inven->SetPos({ 510, 125 });
			Inven->SetVisible(false);
			AddUI(Inven);

			GET_SINGLE(ItemManager)->Init();
		}
	}
	{
		// ���� �г�
		shared_ptr<SettingPanel> settings = make_shared<SettingPanel>();
		if (settings)
		{
			settings->SetVisible(false);
			AddUI(settings);
		}
	}
}

void DevScene::LoadSound()
{
	/*GET_SINGLE(ResourceManager)->LoadSound(L"BGM", L"Sound\\BGM.wav");
	GET_SINGLE(SoundManager)->Play(L"BGM", true, SoundType::BGM);*/
	GET_SINGLE(ResourceManager)->LoadSound(L"Land", L"Sound\\Land.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Button", L"Sound\\Button.wav", SoundType::UI);
	GET_SINGLE(ResourceManager)->LoadSound(L"Sword", L"Sound\\Sword.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Bow", L"Sound\\Bow.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Staff", L"Sound\\Staff.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Potion", L"Sound\\Potion.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Heart", L"Sound\\Heart.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"Teleport", L"Sound\\Teleport.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"SpinAttack", L"Sound\\SpinAttack.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"SpinReady", L"Sound\\SpinReady.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestAccept", L"Sound\\QuestAccept.wav", SoundType::UI);
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestComplete", L"Sound\\QuestComplete.wav", SoundType::UI);
	GET_SINGLE(ResourceManager)->LoadSound(L"QuestFinished", L"Sound\\QuestFinished.wav", SoundType::UI);
	GET_SINGLE(ResourceManager)->LoadSound(L"MonsterOnDamaged", L"Sound\\MonsterOnDamaged.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"PlayerOnDamaged", L"Sound\\PlayerOnDamaged.wav", SoundType::Effect);
	GET_SINGLE(ResourceManager)->LoadSound(L"GameOver", L"Sound\\GameOver.wav", SoundType::UI);
	GET_SINGLE(ResourceManager)->LoadSound(L"Merchant", L"Sound\\Merchant.wav", SoundType::UI);
}

void DevScene::LoadQuest()
{
	{	// ����Ʈ ����Ʈ �޾ƿ���
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_QuestList();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	_questInitialized = true;
}

bool DevScene::IsAnyPopUpVisible()
{
	auto visibleUIs = GetVisibleUIs();
	if (visibleUIs.empty() == false)
	{
		for (auto& ui : visibleUIs)
		{
			auto panel = dynamic_pointer_cast<Panel>(ui);
			if (panel == nullptr)
				continue;

			auto& children = panel->GetChildren();

			if (children.empty())
				continue;

			for (auto& child : children)
			{
				auto popUp = dynamic_pointer_cast<PopUp>(child);

				if (popUp == nullptr)
					continue;

				if (popUp->GetVisible())
					return true;
			}
		}
	}
	return false;
}

void DevScene::SpawnItem(Protocol::ItemInfo info)
{
	shared_ptr<Item> ret = make_shared<Item>();

	ret->SetItemInfo(info);

	ret->SetCellPos({info.posx(), info.posy()}, true);
	ret->info.set_objectid(info.objectid());
	ret->SetState(IDLE);
	AddActor(ret);

	ret->BeginPlay();
}

shared_ptr<GameObject> DevScene::GetObjects(uint64 id)
{
	for (shared_ptr<Actor> actor : _actors[LAYER_OBJECT])
	{
		shared_ptr<GameObject> gameObject = dynamic_pointer_cast<GameObject>(actor);
		if (gameObject == nullptr)
			continue;

		if (gameObject->info.objectid() == id)
			return gameObject;
	}

	return nullptr;
}

shared_ptr<Item> DevScene::GetItem(uint64 id)
{
	for (shared_ptr<Actor> actor : _actors[LAYER_OBJECT])
	{
		shared_ptr<Item> item = dynamic_pointer_cast<Item>(actor);
		if (item == nullptr)
			continue;

		if (item->info.objectid() == id)
			return item;
	}

	return nullptr;
}

shared_ptr<Monster> DevScene::GetMonster()
{
	for (shared_ptr<Actor> actor : _actors[LAYER_OBJECT])
	{
		shared_ptr<Monster> monster = dynamic_pointer_cast<Monster>(actor);
		if (monster)
			return monster;
	}

	return nullptr;
}

void DevScene::Handle_S_AddObject(Protocol::S_AddObject& pkt)
{
	// �ڽſ� ���� ������ �̹� �ִٸ� ��ŵ
	uint64 myplayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
	bool revive = pkt.revive();
	
	// repeated ��Ŷ�� ��ȸ
	const int32 size =pkt.objects_size();
	for (int32 i = 0; i < size; i++)
	{
		const Protocol::ObjectInfo& info = pkt.objects(i);
		if (myplayerId == info.objectid() && revive == false)
			continue;

		if (info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
		{
			auto player = SpawnObject<Player>(Vec2Int{ info.posx(), info.posy() });
			
			// �ִϸ��̼��� ����
			player->info = info;
			player->SetDir(info.dir());
			player->SetState(info.state());
		}
		else if (info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
		{
			switch (info.monstertype())
			{
			case Protocol::MONSTER_TYPE_SNAKE:
			{
				shared_ptr<Snake> snake = SpawnObject<Snake>(Vec2Int{ info.posx(), info.posy() });

				// �ִϸ��̼��� ����
				snake->info = info;
				snake->SetDir(info.dir());
				snake->SetState(info.state());
				break;
			}
			case Protocol::MONSTER_TYPE_MOBLIN:
			{
				// ���
				shared_ptr<Moblin> moblin = SpawnObject<Moblin>(Vec2Int{ info.posx(), info.posy() });

				// �ִϸ��̼��� ����
				moblin->info = info;
				moblin->SetDir(info.dir());
				moblin->SetState(info.state());
				break;
			}
			case Protocol::MONSTER_TYPE_OCTOROC:
			{
				// ���
				shared_ptr<Octoroc> octoroc = SpawnObject<Octoroc>(Vec2Int{ info.posx(), info.posy() });

				// �ִϸ��̼��� ����
				octoroc->info = info;
				octoroc->SetDir(info.dir());
				octoroc->SetState(info.state());
				break;
			}
			}
		}
		else if (info.objecttype() == Protocol::OBJECT_TYPE_NPC)
		{
			switch (info.npctype())
			{
			case Protocol::NPC_TYPE_SIGN:
			{
				auto npc = SpawnObject<Sign>(Vec2Int{ info.posx(), info.posy() });
				// �ִϸ��̼��� ����
				npc->info = info;
				npc->SetDir(DIR_DOWN);
				npc->SetState(info.state());
			}
				break;

			case Protocol::NPC_TYPE_MERCHANT:
			{
				auto npc = SpawnObject<Merchant>(Vec2Int{ info.posx(), info.posy() });
				// �ִϸ��̼��� ����
				npc->info = info;
				npc->SetDir(DIR_DOWN);
				npc->SetState(info.state());
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

		shared_ptr<GameObject> object = GetObjects(id);
		if (object)
			RemoveActor(object);
	}
}

shared_ptr<Player> DevScene::FindClosestPlayer(Vec2Int cellPos)
{
	float best = FLT_MAX;
	shared_ptr<Player> ret = nullptr;

	for (shared_ptr<Actor> actor : _actors[LAYER_OBJECT])
	{
		shared_ptr<Player> player = dynamic_pointer_cast<Player>(actor);
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
				bestScore = (float)score;
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

	shared_ptr<Tilemap> tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return false;

	shared_ptr<Tile> tile = tm->GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// ���� �浹
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	// 1�̸� �� �� ���� ����
	return tile->value != 1;
}

bool DevScene::MonsterCanGo(Vec2Int cellPos)
{
	if (_tilemapActor == nullptr)
		return false;

	shared_ptr<Tilemap> tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return false;

	shared_ptr<Tile> tile = tm->GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// ���� �浹
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	if (tile->value == 2)
		return false;

	// 1�̸� �� �� ���� ����
	return tile->value == 0;
}

bool DevScene::IsSafeZone(Vec2Int cellPos)
{
	if (_tilemapActor == nullptr)
		return false;

	shared_ptr<Tilemap> tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return false;

	auto tile = tm->GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	if (tile->value != 2)
		return false;

	return tile->value == 2;
}

Vec2 DevScene::ConvertPos(Vec2Int cellPos)
{
	Vec2 ret = {};

	if (_tilemapActor == nullptr)
		return ret;

	shared_ptr<Tilemap> tm = _tilemapActor->GetTilemap();
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

	shared_ptr<Tilemap> tm = _tilemapActor->GetTilemap();
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

void DevScene::SetPlayerQuestState(int playerId, int questId, Protocol::QUEST_STATE state)
{
	shared_ptr<MyPlayer> player = GET_SINGLE(SceneManager)->GetMyPlayer();
	player->SetQuestState(questId, state, 0);
}

Protocol::QUEST_STATE DevScene::GetPlayerQuestState(int objectId, int questId)
{
	shared_ptr<MyPlayer> player = GET_SINGLE(SceneManager)->GetMyPlayer();
	if (player)
		return player->GetQuestState(questId);
}

void DevScene::TickMonsterSpawn()
{
	// �� �̻� Ŭ���̾�Ʈ���� ���͸� �������� ����.
	return;

	if (_monsterCount < DESIRED_MONSTER_COUNT)
		SpawnObjectAtRandomPos<Monster>();
}
