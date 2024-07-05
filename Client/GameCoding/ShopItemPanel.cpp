#include "pch.h"
#include "ShopItemPanel.h"
#include "Texture.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "ItemManager.h"
#include "DevScene.h"
#include "MerchantUI.h"
#include "ShopUI.h"
#include "ItemCountsPopUp.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ItemManager.h"
#include "InputManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

ShopItemPanel::ShopItemPanel()
{

}

ShopItemPanel::ShopItemPanel(ITEM item)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	_item = make_shared<ITEM>(item);
}

ShopItemPanel::ShopItemPanel(ITEM item, int index, Vec2 initialPos)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 180,60 });
	_item = make_shared<ITEM>(item);

	_index = index;
	int page = index / 13;

	// 짝수 인덱스는 좌측 줄
	if (index % 2 == 0)
		SetPos(Vec2{ initialPos.x + 5 , initialPos.y + 5 + GetSize().y * ((index % 14) / 2) });
	else
		SetPos(Vec2{ initialPos.x + 185 , initialPos.y + 5 + GetSize().y * ((index % 14) / 2) });

	_rect = RECT{ (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y};
	_item->Rect = _rect;
	_initialPos = _pos;
}

ShopItemPanel::~ShopItemPanel()
{
	_item.reset();
	_rect = {};

	_background = nullptr;
	_goldImage = nullptr;
}

void ShopItemPanel::BeginPlay()
{
	Super::BeginPlay();
}

void ShopItemPanel::Tick()
{
	Super::Tick();

	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _pos.x + _size.x;
	_rect.bottom = _pos.y + _size.y;
}

void ShopItemPanel::Render(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	// Button Background
	::BitBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_background->GetDC(),
		0,
		0,
		SRCCOPY);


	auto sprite = _item->Sprite;
	if (sprite)
	// 아이템 이미지
	{
		::TransparentBlt(hdc,
			_pos.x + 10,
			_pos.y + 10,
			43,
			43,
			sprite->GetDC(),
			sprite->GetPos().x,
			sprite->GetPos().y,
			sprite->GetSize().x,
			sprite->GetSize().y,
			sprite->GetTransparent());
	}

	// 아이템 이름
	{
		RECT _textRect = {_pos.x + 60,_pos.y + 10, _textRect.left + 120, _textRect.top + 18 };
		DrawTextW(hdc, _item->KorName.c_str(), -1, &_textRect, DT_LEFT);
	}

	// 골드 이미지
	{
		::TransparentBlt(hdc,
			_pos.x + 130,
			_pos.y + 35,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetDC(),
			0,
			0,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetTransparent());
	}

	// 아이템 가격
	{
		wstring itemCost = to_wstring(_item->Price);
		RECT _textRect = { _pos.x + 150,_pos.y + 40,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCost.c_str(), -1, &_textRect, DT_CENTER);
	}

	Super::Render(hdc);
}

void ShopItemPanel::OnPopClickAcceptDelegate()
{
}