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
#include "ItemManager.h"

ShopUI::ShopUI()
{

}

ShopUI::~ShopUI()
{

}

void ShopUI::BeginPlay()
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopUIBackground");
	_rect = {};
	// _pos = 135, 80
	{
		_rect.left = _pos.x;
		_rect.top = _pos.y;
		_rect.right = _pos.x + 535;
		_rect.bottom = _pos.y + 450;
	}
	{ // 뒤로 버튼
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + 450, _pos.y + 360 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &ShopUI::OnClickBackButton);
		AddChild(back);
	}

	{ // 대화 종료
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ _pos.x + 450, _pos.y + 410 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &ShopUI::OnClickExitButton);
		AddChild(exit);
	}

	{ // Arrow
		AddSellItem(4);
		int index = GetItemIndex(4);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(4);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // MaxHP+
		AddSellItem(6);
		int index = GetItemIndex(6);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(6);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // 포션
		AddSellItem(5);
		int index = GetItemIndex(5);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(5);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // 투구00
		AddSellItem(12);
		int index = GetItemIndex(12);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(12);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // 투구01
		AddSellItem(13);
		int index = GetItemIndex(13);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(13);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}

	for (auto& child : _children)
		child->BeginPlay();
}

void ShopUI::Tick()
{
	Panel::DragAndMove(&_rect);

	for (auto& child : _children)
	{
		child->Tick();
	}
}

void ShopUI::Render(HDC hdc)
{
	// 배경
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

void ShopUI::AddSellItem(ITEM* item)
{
	_items.push_back(item);
}

void ShopUI::AddSellItem(int itemID)
{
	ITEM* Item = &GET_SINGLE(ItemManager)->GetItem(itemID);
	AddSellItem(Item);
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

ITEM* ShopUI::GetSellItem(int itemID)
{
	for (auto& item : _items)
	{
		if (item->ItemId == itemID)
			return item;
	}
	return nullptr;
}

int ShopUI::GetItemIndex(int itemID)
{
	for (int i = 0; i < _items.size(); i++)
		if (_items[i]->ItemId == itemID)
			return i;

	return -1;
}
