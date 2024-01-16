#include "pch.h"
#include "MerchantUI.h"
#include "Sprite.h"
#include "Button.h"
#include "TextBox.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "NamePlate.h"
#include "TextBox.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

MerchantUI::MerchantUI()
{
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 800;
	_rect.top = 0;
	_rect.bottom = 600;

	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");

	{ // �ڷ� ��ư
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ 200, 500 });
		back->SetSize({ 200, 100 });
		back->AddOnClickDelegate(this, &MerchantUI::OnClickBackButton);
		AddChild(back);
	}

	{ // ��ȭ ����
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 600, 500 });
		exit->SetSize({ 200, 100 });
		exit->AddOnClickDelegate(this, &MerchantUI::OnClickExitButton);
		AddChild(exit);
	}

	{ // ���� ��ư
		Button* shop = new Button();
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Off"), BS_Default);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_On"), BS_Pressed);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Hovered"), BS_Hovered);
		shop->SetPos(Vec2{ 200, 500 });
		shop->SetSize({ 200, 100 });
		shop->AddOnClickDelegate(this, &MerchantUI::OnClickBackButton);
		AddChild(shop);
	}

	{ // ����Ʈ ��ư
		Button* quest = new Button();
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Off"), BS_Default);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_On"), BS_Pressed);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Hovered"), BS_Hovered);
		quest->SetPos(Vec2{ 400, 500 });
		quest->SetSize({ 200, 100 });
		quest->AddOnClickDelegate(this, &MerchantUI::OnClickBackButton);
		AddChild(quest);
	}

	{ // ���� �÷���Ʈ
		NamePlate* namePlate = new NamePlate(L"��� �� ����");
		namePlate->SetPos(Vec2{200, 300});
		AddChild(namePlate);
	}

	{ // ��ȭ ����
		wstring wstr = L"�������!\n�ְ� ǰ���� �����۸� �����ϴ� ��� �� �����Դϴ�.\n������ ���ݿ� ��ð� �ֽ��ϴ�.";
		TextBox* textBox = new TextBox(wstr);
		textBox->SetPos({ 135, 342 });
		textBox->SetSize({ 535, 128 });
		AddChild(textBox);
	}
}

MerchantUI::~MerchantUI()
{

}

void MerchantUI::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void MerchantUI::Tick()
{
	// �г��� visible = true�� �� children ���
	if (GetVisible())
	{
		for (auto& child : _children)
		{
			child->Tick();
		}
	}
}

void MerchantUI::Render(HDC hdc)
{
	for (auto& child : _children)
		child->Render(hdc);
}

void MerchantUI::OnClickBackButton()
{

}

void MerchantUI::OnClickShopButton()
{
	// ���� ȭ�鵵 Child�� �߰��ؾ� ��
}

void MerchantUI::OnClickQuestButton()
{
	// ����Ʈ ȭ�鵵 Child�� �߰��ؾ� ��
}

void MerchantUI::OnClickExitButton()
{
	if (_visible)
	{
		SetVisible(false);
	}
}
