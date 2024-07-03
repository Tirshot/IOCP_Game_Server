#include "pch.h"
#include "MerchantUI.h"
#include "QuestUI.h"
#include "Sprite.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"
#include "Scene.h"
#include "DevScene.h"
#include "ShopUI.h"
#include "ShopItemPanel.h"

MerchantUI::MerchantUI()
{
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 800;
	_rect.top = 0;
	_rect.bottom = 600;

	// 2D ��������Ʈ
	_merchantSprite = GET_SINGLE(ResourceManager)->GetSprite(L"MerchantSprite");
	_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
}

MerchantUI::~MerchantUI()
{
	_merchantSprite = nullptr;
	_backGround = nullptr;
}

void MerchantUI::BeginPlay()
{
	Super::BeginPlay();

	{ // ��ȭ ����
		wstring wstr = L"�������!\n�ְ� ǰ���� �����۸� �����ϴ� ��� �� �����Դϴ�.\n������ ���ݿ� ��ð� �ֽ��ϴ�.";
		auto textBox = make_shared<TextBox>(wstr);
		textBox->SetPos({ 135, 342 });
		textBox->SetSize({ 535, 178 });
		AddChild(textBox);
	}
	{ // ��ȭ ����
		shared_ptr<Button> exit = make_shared<Button>();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 580, 485 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &MerchantUI::OnClickExitButton);
		AddChild(exit);
	}

	{ // ���� ��ư
		shared_ptr<Button> shop = make_shared<Button>();
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Off"), BS_Default);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_On"), BS_Pressed);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Hovered"), BS_Hovered);
		shop->SetPos(Vec2{ 220, 485 });
		shop->SetSize({ 140, 40 });
		shop->AddOnClickDelegate(this, &MerchantUI::OnClickShopButton);
		AddChild(shop);
	}

	{ // ����Ʈ ��ư
		shared_ptr<Button> quest = make_shared<Button>();
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Off"), BS_Default);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_On"), BS_Pressed);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Hovered"), BS_Hovered);
		quest->SetPos(Vec2{ 400, 485 });
		quest->SetSize({ 140, 40 });
		quest->AddOnClickDelegate(this, &MerchantUI::OnClickQuestButton);
		AddChild(quest);
	}
	{ // ���� �÷���Ʈ
		auto namePlate = make_shared<NamePlate>(L"��� �� ����");
		namePlate->SetPos(Vec2{ 200, 300 });
		AddChild(namePlate);
	}

	_initialized = true;
}

void MerchantUI::Tick()
{
	Super::Tick();

	if (_initialized)
	{	// ����Ʈ ����Ʈ �޾ƿ���
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_QuestList();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		_initialized = false;
	}
}

void MerchantUI::Render(HDC hdc)
{
	::TransparentBlt(hdc,
		_pos.x - 200,
		_pos.y + 5,
		_merchantSprite->GetSize().x * 3,
		_merchantSprite->GetSize().y * 3,
		_merchantSprite->GetDC(),
		0,
		0,
		_merchantSprite->GetSize().x,
		_merchantSprite->GetSize().y,
		_merchantSprite->GetTransparent());

	::StretchBlt(hdc,
		135,
		342,
		535,
		178,
		_backGround->GetDC(),
		50,
		50,
		100,
		100,
		SRCCOPY);

	Super::Render(hdc);
}

void MerchantUI::OnClickShopButton()
{
	SetVisible(false);

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	auto shopui = scene->FindUI<ShopUI>(scene->GetUIs());

	if (shopui)
		shopui->SetVisible(true);
}

void MerchantUI::OnClickQuestButton()
{
	SetVisible(false);

	_initialized = true;

	// ����Ʈ Panel Ȱ��ȭ
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	auto questUI = scene->FindUI<QuestUI>(scene->GetUIs());

	if (questUI)
	{
		questUI->ResetQuestList();
		questUI->SetVisible(true);
	}
}

void MerchantUI::OnClickExitButton()
{
	SetVisible(false);
}
