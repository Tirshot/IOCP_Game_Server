#include "pch.h"
#include "MerchantUI.h"
#include "Sprite.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "ResourceManager.h"
#include "SceneManager.h"
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

	{ // 대화 내용
		wstring wstr = L"어서오세요!\n최고 품질의 아이템만 제공하는 언덕 집 상인입니다.\n저렴한 가격에 모시고 있습니다.";
		TextBox* textBox = new TextBox(wstr);
		textBox->SetPos({ 135, 342 });
		textBox->SetSize({ 535, 178 });
		AddChild(textBox);
	}
	{ // 대화 종료
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 580, 485 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &MerchantUI::OnClickExitButton);
		AddChild(exit);
	}

	{ // 상점 버튼
		Button* shop = new Button();
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Off"), BS_Default);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_On"), BS_Pressed);
		shop->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Shop_Hovered"), BS_Hovered);
		shop->SetPos(Vec2{ 220, 485 });
		shop->SetSize({ 140, 40 });
		shop->AddOnClickDelegate(this, &MerchantUI::OnClickShopButton);
		AddChild(shop);
	}

	{ // 퀘스트 버튼
		Button* quest = new Button();
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Off"), BS_Default);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_On"), BS_Pressed);
		quest->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Hovered"), BS_Hovered);
		quest->SetPos(Vec2{ 400, 485 });
		quest->SetSize({ 140, 40 });
		quest->AddOnClickDelegate(this, &MerchantUI::OnClickQuestButton);
		AddChild(quest);
	}
	{ // 네임 플레이트
		NamePlate* namePlate = new NamePlate(L"언덕 집 상인");
		namePlate->SetPos(Vec2{200, 300});
		AddChild(namePlate);
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
	for (auto& child : _children)
		child->Tick();
}

void MerchantUI::Render(HDC hdc)
{
	for (auto& child : _children)
		child->Render(hdc);
}

void MerchantUI::OnClickShopButton()
{
	SetVisible(false);

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	ShopUI* shopui = scene->FindUI<ShopUI>(scene->GetUIs());
	shopui->SetVisible(true);

	// item count reset
	auto* child = shopui->FindChild<ShopItemPanel>(shopui->GetChildren());
	if (child)
		child->ResetItemCount();
}

void MerchantUI::OnClickQuestButton()
{
	SetVisible(false);

	// 퀘스트 Panel 활성화

}

void MerchantUI::OnClickExitButton()
{
	SetVisible(false);


}
