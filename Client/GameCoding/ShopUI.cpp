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

	// 아이템 수량 확인 창
	{
		auto scene = GET_SINGLE(SceneManager)->GetDevScene();
		if (scene)
		{
			_countsPopUp = make_shared<ItemCountsPopUp>();

			if (_countsPopUp)
			{
				_countsPopUp->SetSize({ 300, 180 });
				_countsPopUp->SetPos({ 400, 300 });
				_countsPopUp->AddParentDelegate(this, &ShopUI::OnPopClickAcceptDelegate);
				_countsPopUp->SetVisible(false);
				AddChild(_countsPopUp);
				scene->AddUI(_countsPopUp);
			}
		}
	}

	{
		auto scene = GET_SINGLE(SceneManager)->GetDevScene();
		if (scene)
		{
			// 팝업
			_alert = make_shared<AlertBox>();
			_alert->SetSize({ 250,150 });
			_alert->SetPos({ 400,300 });
			_alert->AddParentDelegate(this, &ShopUI::OnPopClickAlertAcceptDelegate);
			_alert->SetVisible(false);
			AddChild(_alert);
			scene->AddUI(_alert);
		}
	}

	_initialPos = _pos;

	for (auto& child : _children)
		child->BeginPlay();
}

void ShopUI::Tick()
{
	_counts = _countsPopUp->GetCounts();
	_allCost = _counts * _price;

	if (_visible)
	{
		for (auto& child : _children)
			if (child)
				child->Tick();

		auto deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_initializeTime += deltaTime;

		if (_pause)
			return;

		if (IsAnyPopUpVisible())
			return;

		if (IsChildPopUpVisible())
			return;

		if (_initializeTime <= 1.f)
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
					_countsPopUp->SetText(_sellItem->KorName + L"을(를) 몇 개 구입하시겠습니까?");
					_countsPopUp->SetPrice(_price);
					_countsPopUp->SetMaxCounts(99);
					_countsPopUp->SetVisible(true);
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
		alert->SetVisible(true);
		alert->SetInitialPos(alert->GetPos());
		alert->MakeAcceptButton();

		if (twoButtons)
			alert->MakeDenyButton();

		alert->AddParentDelegate(this, func);
		alert->BeginPlay();
	}

	AddChild(alert);

	return alert;
}

void ShopUI::OnPopClickAcceptDelegate()
{
	bool found = false;
	// 구매 확인
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (!myPlayer)
		return;

	if (_sellItem->ItemId <= 3)
		return;

	int gold = myPlayer->info.gold();
	int arrows = myPlayer->info.arrows();
	int maxHP = myPlayer->info.maxhp();
	int myPotion = myPlayer->GetPotionNums();
	int itemCounts = 0;

	if (gold < _allCost)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"골드가 부족합니다.");
		return;
	}

	// 맥스 하트
	if (_sellItem->ItemId == 6)
	{
		if ((maxHP / 10) + _counts > 10)
			return;

		if (gold < _allCost)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + MAX_HP_AMOUNT * _counts, 0, 100));
		myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

		// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			return;
		}
	}

	for (auto& slot : inventory->GetSlots())
	{
		// 구매하려는 아이템이 인벤토리에 이미 있는지 확인
		if (slot->ItemId == _sellItem->ItemId)
		{
			found = true;
			itemCounts = slot->ItemCount;
			break;
		}
	}

	// IsInventoryFull을 사용하여 모든 슬롯이 꽉찼는지 확인
	if (GET_SINGLE(ItemManager)->IsInventoryFull() == true)
	{
		// 인벤토리에 이미 있다면
		if (found)
		{
			// 중첩이 불가능한 타입
			if (_sellItem->Type == L"Wearable")
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(L"인벤토리가 가득 찼습니다.");
				return;
			}
			// 인벤토리가 꽉 찼으며, 중첩이 가능한 타입 -> if문 탈출
		}
		else
		{
			auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
			alert->SetIcon(L"Warning");
			alert->SetText(L"인벤토리가 가득 찼습니다.");
			return;	// 인벤토리가 꽉 찼는데 해당 아이템이 슬롯에 없다면 리턴
		}
	}
	else // 인벤토리가 아직 꽉차지 않았을 때
	{
		if (found)
		{
			int maxCount = _sellItem->MaxCount;

			// 구입하려는 수량이 최대 수량을 초과할 때 리턴
			if (_sellItem->Type != L"Wearable" && itemCounts + _counts > maxCount)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(format(L"아이템의 최대 보유 가능 수량 ({0})개를 초과했습니다.", maxCount));
				return;
			}

			// 갑옷 등 중첩이 불가능한 아이템 갯수가 남은 슬롯보다 많으면 리턴
			if (_sellItem->Type == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(L"인벤토리에 빈 슬롯이 부족합니다.");
				return;
			}
		}
	}

	if (GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts) == false)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"아이템을 구매하지 못했습니다.");
		return;
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

	// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
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
		_countsPopUp->SetText(sellingItem->KorName + L"을(를) 몇 개 판매하시겠습니까?");
		_countsPopUp->SetPrice(gold);
		_countsPopUp->SetMaxCounts(counts);
		_countsPopUp->AddParentDelegate(this, &ShopUI::OnClickSellItemToShopAlertAcceptDelegate);
		_countsPopUp->SetVisible(true);
	}

	SetPause(false);
}

void ShopUI::OnClickSellItemToShopAlertAcceptDelegate()
{
	if (_sellToShop == nullptr)
		return;

	int gold = _sellToShop->Price / 5;
	int counts = _sellToShop->ItemCount;

	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	auto sellingItem = inventory->FindItemFromInventory(_sellToShop);

	inventory->RemoveItem(sellingItem, counts);
	myPlayer->info.set_gold(myPlayer->info.gold() + (gold * counts));
	_sellToShop = nullptr;

	_countsPopUp->AddParentDelegate(this, &ShopUI::OnPopClickAcceptDelegate);

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
