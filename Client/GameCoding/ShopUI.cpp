#include "pch.h"
#include "ShopUI.h"
#include "Texture.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "DevScene.h"
#include "MerchantUI.h"
#include "ShopItemPanel.h"
#include "ResourceManager.h"
#include "SceneManager.h"

ShopUI::ShopUI()
{
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 800;
	_rect.top = 0;
	_rect.bottom = 600;

	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopUIBackground");

	{ // �ڷ� ��ư
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ 585, 440 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &ShopUI::OnClickBackButton);
		AddChild(back);
	}

	{ // ��ȭ ����
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 585, 490 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &ShopUI::OnClickExitButton);
		AddChild(exit);
	}

	{ // Arrow
		Sprite* sprite = GET_SINGLE(ResourceManager)->GetSprite(L"ArrowItem");
		ShopItemPanel* arrow = new ShopItemPanel(sprite, ARROW, 5, (wstring)L"ȭ��", (wstring)L"�ָ����� ������ �� �ְ� ���ִ� ȭ��.", { 140, 85 });
		AddChild(arrow);
	}
	{ // MaxHP+
		Sprite* sprite = GET_SINGLE(ResourceManager)->GetSprite(L"MaxHeartItem");
		ShopItemPanel* maxHeart = new ShopItemPanel(sprite, MAXHEART, 100, (wstring)L"�ƽ� ��Ʈ", (wstring)L"��� �ִ� ü���� 1 ������Ų��.\n�ִ� ü���� 10�� ���� �� ����.", { 140, 185 });
		AddChild(maxHeart);
	}
	{ // ����
		Sprite* sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Potion");
		ShopItemPanel* maxHeart = new ShopItemPanel(sprite, POTION, 15, (wstring)L"ü�� ����", (wstring)L"��� �� ü���� 1 ȸ���Ѵ�.", { 140, 285 });
		AddChild(maxHeart);
	}
}

ShopUI::~ShopUI()
{

}

void ShopUI::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void ShopUI::Tick()
{
	map<int, int> costs;

	for (auto& child : _children)
	{
		child->Tick();

		DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
		ShopItemPanel* item = dynamic_cast<ShopItemPanel*>(child);

		if (item != nullptr)
		{
			costs[item->GetItemType()] = item->GetCost();
		}
	}
	_costs = costs;
}

void ShopUI::Render(HDC hdc)
{
	::BitBlt(hdc,
			_pos.x,
			_pos.y,
			_size.x,
			_size.y,
			_background->GetDC(),
			0,
			0,
			SRCCOPY);

	for (auto& child : _children)
		child->Render(hdc);
}

void ShopUI::OnClickBackButton()
{
	SetVisible(false);

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);
}

void ShopUI::OnClickExitButton()
{
	SetVisible(false);
}