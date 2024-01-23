#include "pch.h"
#include "ShopItemPanel.h"
#include "Texture.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "WeaponSlot.h"
#include "DevScene.h"
#include "MerchantUI.h"
#include "ShopUI.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

ShopItemPanel::ShopItemPanel()
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	{ // LL Left
		Button* LLeft = new Button();
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Default);
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Pressed);
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Hovered);
		LLeft->SetPos(Vec2{ _pos.x + 105, _pos.y + 68 });
		LLeft->SetSize({ 32, 21 });
		LLeft->AddOnClickDelegate(this, &ShopItemPanel::OnClickLLButton);
		AddChild(LLeft);
	}
	{ // Left
		Button* Left = new Button();
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Default);
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Pressed);
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Hovered);
		Left->SetPos(Vec2{ _pos.x + 147, _pos.y + 68 });
		Left->SetSize({ 32, 21 });
		Left->AddOnClickDelegate(this, &ShopItemPanel::OnClickLButton);
		AddChild(Left);
	}
	{ // Right
		Button* Right = new Button();
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Default);
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Pressed);
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Hovered);
		Right->SetPos(Vec2{ _pos.x + 227, _pos.y + 68 });
		Right->SetSize({ 32, 21 });
		Right->AddOnClickDelegate(this, &ShopItemPanel::OnClickRButton);
		AddChild(Right);
	}
	{ // RR Right
		Button* RRight = new Button();
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Default);
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Pressed);
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Hovered);
		RRight->SetPos(Vec2{ _pos.x + 267, _pos.y + 68 });
		RRight->SetSize({ 32, 21 });
		RRight->AddOnClickDelegate(this, &ShopItemPanel::OnClickRRButton);
		AddChild(RRight);
	}
	{	// Item
		_itemImage = GET_SINGLE(ResourceManager)->GetSprite(L"ArrowItem");
		_itemCost = 9999;
		_itemName = L"기본 아이템";
		_itemText = L"기본 아이템 설명 문구입니다.";
	}
}

ShopItemPanel::ShopItemPanel(Sprite* sprite, Protocol::ITEM_TYPE itemtype, int cost, wstring name, wstring text, Vec2 pos)
{
	_pos = pos;
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	{	// ITEM
		_itemImage = sprite;
		_itemType = itemtype;
		_itemCost = cost;
		_itemName = name;
		_itemText = text;
	}
	{ // LL Left
		Button* LLeft = new Button();
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Default);
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Pressed);
		LLeft->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), BS_Hovered);
		LLeft->SetPos(Vec2{ pos.x + 87, pos.y + 78 });
		LLeft->SetSize({ 32, 21 });
		LLeft->AddOnClickDelegate(this, &ShopItemPanel::OnClickLLButton);
		AddChild(LLeft);
	}
	{ // Left
		Button* Left = new Button();
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Default);
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Pressed);
		Left->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), BS_Hovered);
		Left->SetPos(Vec2{ pos.x + 127, pos.y + 78 });
		Left->SetSize({ 32, 21 });
		Left->AddOnClickDelegate(this, &ShopItemPanel::OnClickLButton);
		AddChild(Left);
	}
	{ // Right
		Button* Right = new Button();
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Default);
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Pressed);
		Right->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), BS_Hovered);
		Right->SetPos(Vec2{ pos.x + 207, pos.y + 78 });
		Right->SetSize({ 32, 21 });
		Right->AddOnClickDelegate(this, &ShopItemPanel::OnClickRButton);
		AddChild(Right);
	}
	{ // RR Right
		Button* RRight = new Button();
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Default);
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Pressed);
		RRight->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), BS_Hovered);
		RRight->SetPos(Vec2{ pos.x + 247, pos.y + 78 });
		RRight->SetSize({ 32, 21 });
		RRight->AddOnClickDelegate(this, &ShopItemPanel::OnClickRRButton);
		AddChild(RRight);
	}
	{ // 구매
		Button* purchase = new Button();
		purchase->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PurchaseButton"), BS_Default);
		purchase->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PurchaseButton"), BS_Pressed);
		purchase->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PurchaseButton"), BS_Hovered);
		purchase->SetPos(Vec2{ pos.x + 340, pos.y + 78 });
		purchase->SetSize({ 32, 21 });
		purchase->AddOnClickDelegate(this, &ShopItemPanel::OnClickPurchaseButton);
		AddChild(purchase);
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
	for (auto& child : _children)
		child->Tick();

	_allCost = _itemCount * _itemCost;
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
		_itemImage->GetDC(),
		0,
		0,
		_itemImage->GetSize().x,
		_itemImage->GetSize().y,
		_itemImage->GetTransparent());

	for (auto& child : _children)
		child->Render(hdc);

	// 아이템 갯수
	{
		wstring itemCount = to_wstring(_itemCount);
		RECT _textRect = { _pos.x + 152,_pos.y + 70,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCount.c_str(), -1, &_textRect, DT_CENTER);
	}

	// 아이템 이름
	{
		RECT _textRect = {_pos.x + 70,_pos.y + 10, _textRect.left + 90, _textRect.top + 18 };
		DrawTextW(hdc, _itemName.c_str(), -1, &_textRect, DT_LEFT);
	}

	// 아이템 설명
	{
		RECT _textRect = { _pos.x + 70,_pos.y + 30,_textRect.left + 290, _textRect.top + 40 };
		DrawTextW(hdc, _itemText.c_str(), -1, &_textRect, DT_LEFT);
	}

	// 골드 이미지
	{
		::TransparentBlt(hdc,
			_pos.x + 6,
			_pos.y + 65,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetDC(),
			0,
			0,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetTransparent());

		::TransparentBlt(hdc,
			_pos.x + 270,
			_pos.y + 65,
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
		wstring itemCost = to_wstring(_itemCost);
		RECT _textRect = { _pos.x + 25,_pos.y + 70,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCost.c_str(), -1, &_textRect, DT_CENTER);
	}

	// 칸 내 아이템 총 가격
	{
		wstring itemCost = to_wstring(_allCost);
		RECT _textRect = { _pos.x + 290,_pos.y + 70,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCost.c_str(), -1, &_textRect, DT_CENTER);
	}
}

void ShopItemPanel::OnClickLLButton()
{
	_itemCount = clamp(_itemCount - 5 , 0, 99);
}

void ShopItemPanel::OnClickLButton()
{
	_itemCount = clamp(_itemCount - 1, 0, 99);
}

void ShopItemPanel::OnClickRRButton()
{
	_itemCount = clamp(_itemCount + 5, 0, 99);
}

void ShopItemPanel::OnClickRButton()
{
	_itemCount = clamp(_itemCount + 1, 0, 99);
}

void ShopItemPanel::OnClickPurchaseButton()
{
	if (_allCost <= 0)
		return;

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer)
	{
		int gold = myPlayer->info.gold();
		int arrows = myPlayer->info.arrows();
		int maxHP = myPlayer->info.maxhp();
		int myPotion = myPlayer->GetPotionNums();

		if (gold < _allCost)
			return;

		switch (_itemType)
		{
		case ARROW:
			myPlayer->info.set_arrows(arrows + _itemCount);
			break;

		case MAXHEART:
			if (maxHP + _itemCount > 10)
				return;
			myPlayer->info.set_maxhp(clamp(maxHP + _itemCount, 0, 10));
			break;

		case POTION:
			if (myPotion >= 9)
				return;
			myPlayer->info.set_potion(myPlayer->info.potion() + _itemCount);
			break;
		}
		myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);
		ResetItemCount();
		// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
	}
}
