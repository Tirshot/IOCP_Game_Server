#include "pch.h"
#include "ShopItemPanel.h"
#include "Texture.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "WeaponSlot.h"
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

ShopItemPanel::ShopItemPanel(ITEM* item)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	_item = item;
}

ShopItemPanel::ShopItemPanel(ITEM* item, int index, Vec2 initialPos)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 180,60 });
	_item = item;

	// 짝수 인덱스는 좌측 줄
	if (index % 2 == 0)
		SetPos(Vec2{ initialPos.x + 5 , initialPos.y + 5 + GetSize().y * (index / 2) });
	else
		SetPos(Vec2{ initialPos.x + 185 , initialPos.y + 5 + GetSize().y * (index / 2) });

	_rect = RECT{ (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y};
	_item->Rect = _rect;

	// 아이템 이름
	{
		_itemName = new TextBox();
		_itemName->SetText(_item->KorName);
		_itemName->SetSize(Vec2Int{ 160 , 30 });
		_itemName->SetPadding(0, 10);
		_itemName->AlignText(TextAlign::Center);
		_itemName->SetVisible(false);
		_itemName->SetPos(Vec2{ initialPos.x + 370, initialPos.y + 5 });
		AddChild(_itemName);
	}

	// 아이템 설명
	{
		_description = new TextBox();
		_description->SetText(_item->Description);
		_description->SetSize(Vec2Int{ 160 , 250 });
		_description->SetPadding(5, 5);
		_description->SetVisible(false);
		_description->SetPos(Vec2{ initialPos.x + 370, initialPos.y + 35});
		AddChild(_description);
	}
}

ShopItemPanel::~ShopItemPanel()
{
}

void ShopItemPanel::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void ShopItemPanel::Tick()
{
	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _pos.x + _size.x;
	_rect.bottom = _pos.y + _size.y;

	for (auto& child : _children)
		child->Tick();
	
	// 아이템 설명
	if (IsMouseInRect(_rect))
	{
		_itemName->SetVisible(true);
		_description->SetVisible(true);
	}
	else
	{
		_itemName->SetVisible(false);
		_description->SetVisible(false);
	}
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

	// 아이템 이미지
	::TransparentBlt(hdc,
		_pos.x + 10,
		_pos.y + 10,
		43,
		43,
		_item->Sprite->GetDC(),
		_item->Sprite->GetPos().x,
		_item->Sprite->GetPos().y,
		_item->Sprite->GetSize().x,
		_item->Sprite->GetSize().y,
		_item->Sprite->GetTransparent());

	// 아이템 이름
	{
		RECT _textRect = {_pos.x + 60,_pos.y + 10, _textRect.left + 90, _textRect.top + 18 };
		DrawTextW(hdc, _item->KorName.c_str(), -1, &_textRect, DT_LEFT);
	}

	// 골드 이미지
	{
		//::TransparentBlt(hdc,
		//	_pos.x + 6,
		//	_pos.y + 65,
		//	_goldImage->GetSize().x / 3,
		//	_goldImage->GetSize().y,
		//	_goldImage->GetDC(),
		//	0,
		//	0,
		//	_goldImage->GetSize().x / 3,
		//	_goldImage->GetSize().y,
		//	_goldImage->GetTransparent());

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

	//// 칸 내 아이템 총 가격
	//{
	//	wstring itemCost = to_wstring(_allCost);
	//	RECT _textRect = { _pos.x + 290,_pos.y + 70,_textRect.left + 30,_textRect.top + 18 };
	//	DrawTextW(hdc, itemCost.c_str(), -1, &_textRect, DT_CENTER);
	//}

	for (auto& child : _children)
		if (child->GetVisible() == true)
			child->Render(hdc);
}

void ShopItemPanel::OnPopClickAcceptDelegate()
{
}

//void ShopItemPanel::OnClickPurchaseButton()
//{
//	if (_allCost <= 0)
//		return;
//
//	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
//	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
//
//	if (myPlayer)
//	{
//		int gold = myPlayer->info.gold();
//		int arrows = myPlayer->info.arrows();
//		int maxHP = myPlayer->info.maxhp();
//		int myPotion = myPlayer->GetPotionNums();
//
//		if (gold < _allCost)
//			return;
//
//		switch (_itemType)
//		{
//		case ARROW:
//			myPlayer->info.set_arrows(arrows + _itemCount);
//			GET_SINGLE(ItemManager)->AddItemToInventory(4);
//			break;
//
//		case MAXHEART:
//			if (maxHP + _itemCount > 10)
//				return;
//			myPlayer->info.set_maxhp(clamp(maxHP + _itemCount, 0, 10));
//			break;
//
//		case POTION:
//			if (myPotion >= 9)
//				return;
//			myPlayer->info.set_potion(myPlayer->info.potion() + _itemCount);
//			GET_SINGLE(ItemManager)->AddItemToInventory(5);
//			break;
//		}
//		myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);
//		ResetItemCount();
//		// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
//		{
//			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
//			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
//		}
//	}
//}
