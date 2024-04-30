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
		Button* back = new Button();
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
		Button* exit = new Button();
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
		Button* minus = new Button();
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
		Button* plus = new Button();
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
		// 투구00
		AddSellItem(12);
		// 투구01
		AddSellItem(13);
		// 투구01
		AddSellItem(14);
		// 투구01
		AddSellItem(15);
		// 투구01
		AddSellItem(16);
		// 투구01
		AddSellItem(17);
		AddSellItem(18);
		AddSellItem(19);
		AddSellItem(20);
		AddSellItem(21);
		AddSellItem(22);
		AddSellItem(23);
		AddSellItem(25);
		AddSellItem(26);
		AddSellItem(27);
		AddSellItem(28);
		AddSellItem(29);
		AddSellItem(30);
		AddSellItem(31);
		AddSellItem(32);
		AddSellItem(33);
		AddSellItem(34);
		AddSellItem(35);
		AddSellItem(36);
		AddSellItem(37);
		AddSellItem(38);
		AddSellItem(39);
		AddSellItem(40);
		AddSellItem(1);
		AddSellItem(2);
		AddSellItem(3);
	}

	// 아이템 수량 확인 창
	{
		_countsPopUp = new ItemCountsPopUp();
		if (_countsPopUp)
		{
			_countsPopUp->SetSize({ 300, 180 });
			_countsPopUp->SetPos({ 400, 300 });
			AddChild(_countsPopUp);
			_countsPopUp->AddParentDelegate(this, &ShopUI::OnPopClickAcceptDelegate);
			_countsPopUp->SetVisible(false);
		}
	}

	{	// 팝업
		_alert = new AlertBox();
		_alert->SetSize({ 250,150 });
		_alert->SetPos({400,300});
		_alert->SetVisible(false);
		AddChild(_alert);
	}

	_initialPos = _pos;

	for (auto& child : _children)
		child->BeginPlay();
}

void ShopUI::Tick()
{
	Panel::DragAndMove(&_rect);

	if (_visible)
	{
		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_initializeTime += deltaTime;
		{
			_rect.left = _pos.x;
			_rect.top = _pos.y;
			_rect.right = _pos.x + 535;
			_rect.bottom = _pos.y + 450;
		}
		_dragRect = { (int)_pos.x + 370, (int)_pos.y, (int)_pos.x + 535, (int)_pos.y + 300 };

		for (auto& child : _children)
		{
			child->Tick();

			auto itemPanel = dynamic_cast<ShopItemPanel*>(child);

			if (itemPanel == nullptr)
				continue;

			// 팝업 창 위에서 다른 칸이 눌리는 버그 수정
			if (_countsPopUp->GetVisible() == true)
				continue;

			// 팝업 창 위에서 다른 칸이 눌리는 버그 수정
			if (_alert->GetVisible() == true)
				continue;

			// 판매창이 열리는 순간 품목이 눌리는 버그 수정
			if (_initializeTime <= 0.5f)
				continue;

			auto* Item = dynamic_cast<ShopItemPanel*>(child);
			if (Item)
			{
				int index = Item->GetItemIndex();
				int page = 1 + index / 14;

				// 보고있는 페이지와 아이템이 있는 페이지가 동일하지 않으면 건너뜀
				if (_page != page)
					continue;
			}

			if (IsMouseInRect(itemPanel->GetRect()))
			{
				if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
				{
					_sellItem = itemPanel->GetItem();
					_price = _sellItem->Price;
					_countsPopUp->SetText(_sellItem->KorName + L"을(를) 몇 개 구입하시겠습니까?");
					_countsPopUp->SetPrice(_price);
					_countsPopUp->SetVisible(true);
					_initializeTime = 0.f;
				}
			}
		}
		_counts = _countsPopUp->GetCounts();
		_allCost = _counts * _price;
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
		auto* Item = dynamic_cast<ShopItemPanel*>(child);
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

void ShopUI::AddSellItem(ITEM* item)
{
	_items.push_back(item);
}

void ShopUI::AddSellItem(int itemID)
{
	ITEM* Item = &GET_SINGLE(ItemManager)->GetItem(itemID);
	AddSellItem(Item);
	int index = GetItemIndex(itemID);
	ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(itemID);
	ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
	AddChild(itemPanel);
}

void ShopUI::OnPopClickAcceptDelegate()
{
	bool found = false;
	// 구매 확인
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	Inventory* inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (!myPlayer)
		return;

	int gold = myPlayer->info.gold();
	int arrows = myPlayer->info.arrows();
	int maxHP = myPlayer->info.maxhp();
	int myPotion = myPlayer->GetPotionNums();

	if (gold < _allCost)
	{
		_alert->SetIcon(L"Warning");
		_alert->SetText(L"골드가 부족합니다.");
		_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
		_alert->SetVisible(true);
		return;
	}

	// 맥스 하트
	if (_sellItem->ItemId == 6)
	{
		if (maxHP + _counts > 10)
			return;

		if (gold < _allCost)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + _counts, 0, 10));
		myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

		// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			return;
		}
	}

	// IsInventoryFull을 사용하여 모든 슬롯이 꽉찼는지 확인
	if (GET_SINGLE(ItemManager)->IsInventoryFull() == true)
	{
		for (auto& slot : inventory->GetSlots())
		{
			// 구매하려는 아이템이 인벤토리에 이미 있는지 확인
			if (slot.ItemId == _sellItem->ItemId)
			{
				found = true;
				break;
			}
		}

		// 인벤토리에 이미 있다면
		if (found)
		{
			// 중첩이 불가능한 타입
			if (_sellItem->Type == L"Wearable")
			{
				_alert->SetIcon(L"Warning");
				_alert->SetText(L"인벤토리가 가득 찼습니다.");
				_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
				_alert->SetVisible(true);
				return;
			}
			// 인벤토리가 꽉 찼으며, 중첩이 가능한 타입 -> if문 탈출
		}
		else
		{
			_alert->SetIcon(L"Warning");
			_alert->SetText(L"인벤토리가 가득 찼습니다.");
			_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
			_alert->SetVisible(true);
			return;	// 인벤토리가 꽉 찼는데 해당 아이템이 슬롯에 없다면 리턴
		}
	}
	else // 인벤토리가 아직 꽉차지 않았을 때
	{
		// 아이템을 추가하면 인벤토리가 가득 차게 되는지 확인
		for (auto& slot : inventory->GetSlots())
		{
			// 구매하려는 아이템이 인벤토리에 이미 있는지 확인
			if (slot.ItemId == _sellItem->ItemId)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			// 갑옷 등 중첩이 불가능한 아이템 갯수가 남은 슬롯보다 많으면 리턴
			if (_sellItem->Type == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
			{
				_alert->SetIcon(L"Warning");
				_alert->SetText(L"인벤토리가 부족합니다.");
				_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
				_alert->SetVisible(true);
				return;
			}
		}
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

	// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}

	GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts);
}

void ShopUI::OnClickBackButton()
{
	// 팝업 창 위에서 다른 버튼이 눌리는 버그 수정
	if (_countsPopUp->GetVisible() == true)
		return;

	// 팝업 창 위에서 다른 버튼이 눌리는 버그 수정
	if (_alert->GetVisible() == true)
		return;

	SetVisible(false);
	SetChildVisible(false);
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	ResetPos();
	_initializeTime = 0.f;

	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);
}

void ShopUI::OnClickExitButton()
{
	// 팝업 창 위에서 다른 버튼이 눌리는 버그 수정
	if (_countsPopUp->GetVisible() == true)
		return;

	// 팝업 창 위에서 다른 버튼이 눌리는 버그 수정
	if (_alert->GetVisible() == true)
		return;

	SetVisible(false);
	SetChildVisible(false);
	ResetPos();
	_initializeTime = 0.f;
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

void ShopUI::SetChildVisible(bool visible)
{
	for (auto& child : _children)
	{
		child->SetVisible(visible);

		// 버튼은 부모의 visiblity에 영향을 받음
		auto button = dynamic_cast<Button*>(child);

		if (button)
			button->SetVisible(true);
	}
}

void ShopUI::OnClickCountMinusButton()
{
	_page--;
	_page = ::clamp(_page, 1, 1 + ((int)_items.size() / 13));
}

void ShopUI::OnClickCountPlusButton()
{
	_page++;
	_page = ::clamp(_page, 1, 1 + ((int)_items.size() / 13));
}
