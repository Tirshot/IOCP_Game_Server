#include "pch.h"
#include "ShopUI.h"
#include "Texture.h"
#include "Button.h"
#include "TextBox.h"
#include "AlertBox.h"
#include "NamePlate.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "MerchantUI.h"
#include "ShopItemPanel.h"
#include "ItemCountsPopUp.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "TimeManager.h"

ShopUI::ShopUI()
{

}

ShopUI::~ShopUI()
{
	_children.clear();
	_items.clear();

	_sellItem.reset();
	_sellToShop.reset();

	_background.reset();
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
	_dragRect = {(int)_pos.x + 370, (int)_pos.y, (int)_pos.x + 535, (int)_pos.y + 300};

	{ // 뒤로 버튼
		shared_ptr<Button> back = make_shared<Button>();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + 450, _pos.y + 360 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &ShopUI::OnClickBackButton);
		back->SetInitialPos(back->GetPos());
		AddChild(back);
	}

	{ // 대화 종료
		shared_ptr<Button> exit = make_shared<Button>();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ _pos.x + 450, _pos.y + 410 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &ShopUI::OnClickExitButton);
		exit->SetInitialPos(exit->GetPos());
		AddChild(exit);
	}

	// 페이지 감소
	{
		shared_ptr<Button> minus = make_shared<Button>();
		minus->SetPos({ _pos.x + 400, _pos.y + 300 });
		minus->SetSize({ 40,24 });
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Default);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Hovered);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Clicked);
		minus->AddOnClickDelegate(this, &ShopUI::OnClickCountMinusButton);
		minus->SetInitialPos(minus->GetPos());
		AddChild(minus);
	}

	// 페이지 증가
	{
		shared_ptr<Button> plus = make_shared<Button>();
		plus->SetPos({ _pos.x + 500, _pos.y + 300 });
		plus->SetSize({ 40,24 });
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Default);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Hovered);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Clicked);
		plus->AddOnClickDelegate(this, &ShopUI::OnClickCountPlusButton);
		plus->SetInitialPos(plus->GetPos());
		AddChild(plus);
	}

	// 아이템 이름
	{
		_itemName = make_shared<TextBox>();
		_itemName->SetText(L"");
		_itemName->SetSize(Vec2Int{ 160 , 30 });
		_itemName->SetPadding(0, 10);
		_itemName->AlignText(TextAlign::Center);
		_itemName->SetVisible(false);
		_itemName->SetPos(Vec2{ 503, 85 });
		_itemName->SetInitialPos(Vec2{ 503, 85 });
		AddChild(_itemName);
	}

	// 아이템 설명
	{
		_description = make_shared<TextBox>();
		_description->SetText(L"");
		_description->SetSize(Vec2Int{ 160 , 250 });
		_description->SetPadding(5, 5);
		_description->SetVisible(false);
		_description->SetPos(Vec2{ 503, 115 });
		_description->SetInitialPos(Vec2{ 503, 115 });
		AddChild(_description);
	}
	{ 
		// Arrow
		AddSellItem(4);

		// MaxHP+
		AddSellItem(6);

		// 포션
		AddSellItem(5);

		// 장비
		AddSellItem(10);
		AddSellItem(11);
		AddSellItem(12);
		AddSellItem(13);

		AddSellItem(14);
		AddSellItem(15);
		AddSellItem(16);
		AddSellItem(17);

		AddSellItem(18);
		AddSellItem(19);
		AddSellItem(20);
		AddSellItem(21);

		AddSellItem(22);
		AddSellItem(23);
		AddSellItem(24);
		AddSellItem(25);

		// 기타
		AddSellItem(26);
		AddSellItem(1);
		AddSellItem(2);
		AddSellItem(3);
	}

	_initialPos = _pos;

	Super::BeginPlay();
}

void ShopUI::Tick()
{
	_allCost = _counts * _price;

	if (_visible)
	{
		Super::Tick();

		if (_pause)
			return;

		auto deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_initializeTime += deltaTime;

		if (IsAnyPopUpVisible())
			return;

		if (IsChildPopUpVisible())
			return;

		if (_initializeTime <= 0.5f)
			return;

		{
			_rect.left = _pos.x;
			_rect.top = _pos.y;
			_rect.right = _pos.x + 535;
			_rect.bottom = _pos.y + 450;
		}

		for (auto& child : _children)
		{
			auto Item = dynamic_pointer_cast<ShopItemPanel>(child);
			if (Item == nullptr)
				continue;

			RECT ItemRect = Item->GetRect();
			int index = Item->GetItemIndex();
			int page = 1 + index / 14;

			// 보고있는 페이지와 아이템이 있는 페이지가 동일하지 않으면 건너뜀
			if (_page != page)
				continue;

			if (IsMouseInRect(ItemRect))
			{
				// 이름 및 설명 노출
				auto itemPanel = dynamic_pointer_cast<ShopItemPanel>(child);
				if (itemPanel)
				{
					auto item = itemPanel->GetItem();
					if (item)
					{
						auto name = item->KorName;
						auto description = item->Description;
						_itemName->SetText(name);
						_description->SetText(description);
					}
				}
				else
				{
					_itemName->SetText(L"");
					_description->SetText(L"");
				}

				// 구매 로직
				auto inventory = GET_SINGLE(ItemManager)->GetInventory();
				if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
				{
					if (inventory && inventory->GetVisible() == true)
					{
						RECT invenRect = inventory->GetInvenRect();

						if (IsRectOverlapped(ItemRect, invenRect))
							return;
					}

					_sellItem = Item->GetItem();
					_price = _sellItem->Price;

					auto counts = MakeCountsBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 180 }, _sellItem->ItemId, &ShopUI::OnPopClickAcceptDelegate);
					counts->SetText(_sellItem->KorName + L"을(를) 몇 개 구입하시겠습니까?");
					counts->SetPrice(_price);

					auto item = GET_SINGLE(ItemManager)->FindItemFromInventory(_sellItem->ItemId);
					if (item)
					{
						auto ownedCount = item->ItemCount;
						counts->SetMaxCounts(_sellItem->MaxCount - ownedCount);
					}
					else
					{
						counts->SetMaxCounts(_sellItem->MaxCount);
					}

					SetPause(true);
					_initializeTime = 0.f;
					return;
				}
			}
		}
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

	// 아이템 갯수
	{	
		SetTextColor(hdc, RGB(255, 255, 255));
		wstring Pages = to_wstring(_page) + L" / " + to_wstring(1 + (int)(_items.size() / 13));
		RECT _textRect = { _pos.x + 435, _pos.y + 290,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, Pages.c_str(), -1, &_textRect, DT_CENTER);
	}

	for (auto& child : _children)
	{
		auto Item = dynamic_pointer_cast<ShopItemPanel>(child);
		if (Item)
		{
			int index = Item->GetItemIndex();
			int page = 1 + index / 14;

			// 보고있는 페이지와 아이템이 있는 페이지가 동일하지 않으면 건너뜀
			if (_page != page)
				continue;
		}
		// 페이지가 동일하거나 ShopItemPanel이 아니면 렌더링
		child->Render(hdc);
	}
}

void ShopUI::AddSellItem(shared_ptr<ITEM> item)
{
	_items.push_back(item);
}

void ShopUI::AddSellItem(int itemID)
{
	auto Item = GET_SINGLE(ItemManager)->GetItem(itemID);
	AddSellItem(make_shared<ITEM>(Item));
	int index = GetItemIndex(itemID);
	auto itemInfo = GET_SINGLE(ItemManager)->GetItem(itemID);
	auto itemPanel = make_shared<ShopItemPanel>(itemInfo, index, _pos);
	AddChild(itemPanel);
}

void ShopUI::SellItemToShop(shared_ptr<ITEM> item)
{
	_sellToShop = item;

	int itemID = _sellToShop->ItemId;
	if (itemID == 1 || itemID == 2 || itemID == 3)
	{
		auto alert = MakeAlertBox(Vec2{ GWinSizeX / 2, GWinSizeY / 2 }, { 300,150 }, &ShopUI::OnClickSellItemToShopAlertAcceptDelegate, false);
		SetPause(true);

		alert->SetText(L"기본 무기는 판매할 수 없습니다.");
		alert->SetIcon(L"Alert");
	}
	else
	{
		auto alert = MakeAlertBox(Vec2{ GWinSizeX / 2, GWinSizeY / 2 }, { 300,150 }, &ShopUI::OnClickSellItemToShopAlertAcceptDelegate);
		SetPause(true);

		alert->SetText(item->KorName + L"을(를) 개당 " + to_wstring(_sellToShop->Price / _buyPriceDivider) +L"골드에 판매하시겠습니까 ? ");
		alert->SetIcon(L"Warning");
	}
}

shared_ptr<AlertBox> ShopUI::MakeAlertBox(Vec2 pos, Vec2Int size, void(ShopUI::* func)(), bool twoButtons)
{
	// 팝업
	shared_ptr<AlertBox> alert = make_shared<AlertBox>();
	if (alert)
	{
		// AlertBox 초기화
		alert->SetSize(size);
		alert->SetPos({ pos.x, pos.y });
		alert->SetInitialPos(alert->GetPos());
		alert->MakeAcceptButton();

		if (twoButtons)
			alert->MakeDenyButton();

		alert->AddParentDelegate(this, func);
		alert->BeginPlay();
		alert->SetVisible(true);
	}

	AddChild(alert);

	return alert;
}

shared_ptr<ItemCountsPopUp> ShopUI::MakeCountsBox(Vec2 pos, Vec2Int size, int itemID, void(ShopUI::* func)())
{
	shared_ptr<ItemCountsPopUp> counts = make_shared<ItemCountsPopUp>();
	if (counts)
	{
		// AlertBox 초기화
		counts->SetSize(size);
		counts->SetPos({ pos.x, pos.y });
		counts->SetInitialPos(counts->GetPos());
		counts->MakeAcceptButton();
		counts->MakeDenyButton();
		counts->SetItemID(itemID);
		counts->AddParentDelegate(this, func);
		counts->BeginPlay();
		counts->SetVisible(true);
	}

	AddChild(counts);

	return counts;
}

void ShopUI::OnPopClickAcceptDelegate()
{
	SetPause(false);
}

void ShopUI::OnPopClickAlertAcceptDelegate()
{
	if (_sellToShop == nullptr)
		return;

	int gold = _sellToShop->Price / _buyPriceDivider;
	int counts = _sellToShop->ItemCount;

	// 구매 확인
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (!myPlayer)
		return;

	if (!inventory)
		return;

	auto sellingItem = inventory->FindItemFromInventory(_sellToShop);
	if (!sellingItem)
		return;

	int itemID = sellingItem->ItemId;
	if (itemID == 1 || itemID == 2 || itemID == 3)
		return;

	if (counts == 1)
	{
		inventory->RemoveItem(sellingItem);
		myPlayer->info.set_gold(myPlayer->info.gold() + gold);
		_sellToShop = nullptr;
	}
	else
	{
		auto countBox = MakeCountsBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 180 }, itemID, &ShopUI::OnClickSellItemToShopAlertAcceptDelegate);
		countBox->SetText(sellingItem->KorName + L"을(를) 몇 개 판매하시겠습니까?");
		countBox->SetPrice(gold);
		countBox->SetMaxCounts(counts);
	}

	SetPause(false);
}

void ShopUI::OnClickSellItemToShopAlertAcceptDelegate()
{
	if (_sellToShop == nullptr)
		return;

	int gold = _sellToShop->Price / _buyPriceDivider;
	int counts = _sellToShop->ItemCount;

	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	auto sellingItem = inventory->FindItemFromInventory(_sellToShop);

	inventory->RemoveItem(sellingItem, counts);
	myPlayer->info.set_gold(myPlayer->info.gold() + (gold * counts));
	_sellToShop = nullptr;

	SetPause(false);
}

void ShopUI::OnClickBackButton()
{
	if (IsChildPopUpVisible())
		return;

	SetVisible(false);
	SetChildVisible(false);
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	ResetPos();
	_initializeTime = 0.f;
	_page = 1;

	auto merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);

	SetPause(false);
}

void ShopUI::OnClickExitButton()
{
	if (IsChildPopUpVisible())
		return;

	SetVisible(false);
	SetChildVisible(false);

	ResetPos();
	_initializeTime = 0.f;
	_page = 1;

	SetPause(false);
}

shared_ptr<ITEM> ShopUI::GetSellItem(int itemID)
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

void ShopUI::SetChildVisible(bool visible)
{
	for (auto& child : _children)
	{
		child->SetVisible(visible);

		// 버튼은 부모의 visiblity에 영향을 받음
		auto button = dynamic_pointer_cast<Button>(child);

		if (button)
			button->SetVisible(true);
	}
}

void ShopUI::OnClickCountMinusButton()
{
	if (IsChildPopUpVisible())
		return;

	_page--;
	_page = ::clamp(_page, 1, 1 + ((int)_items.size() / 13));
}

void ShopUI::OnClickCountPlusButton()
{
	if (IsChildPopUpVisible())
		return;

	_page++;
	_page = ::clamp(_page, 1, 1 + ((int)_items.size() / 13));
}
