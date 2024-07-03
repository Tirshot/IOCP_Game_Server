#include "pch.h"
#include "ItemCountsPopUp.h"
#include "Sprite.h"
#include "UI.h"
#include "TextBox.h"
#include "AlertBox.h"
#include "MyPlayer.h"
#include "Inventory.h"
#include "Button.h"
#include "InputManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ResourceManager.h"
#include "ClientPacketHandler.h"

ItemCountsPopUp::ItemCountsPopUp()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");
	_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
}

ItemCountsPopUp::~ItemCountsPopUp()
{
	_background = nullptr;
	_icon = nullptr;
}

void ItemCountsPopUp::BeginPlay()
{
	Super::BeginPlay();

	// 기즈모 위치를 중앙에 위치시키기 위해서는 Size 설정을 Pos 설정보다 먼저!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };
	_initialPos = _pos;
	// 텍스트
	{
		shared_ptr<TextBox> text = make_shared<TextBox>();
		text->SetText(L"테스트용 텍스트입니다. \n두번째 줄입니다. \n 세번째 줄입니다.");
		text->SetPos(Vec2{ _pos.x + 60, _pos.y + 20 });
		text->SetSize(Vec2Int{ _size.x - 80, (_size.y / 2) - 25 });
		text->SetPadding(3, 3);
		text->SetInitialPos(text->GetPos());
		AddChild(text);
	}

	// 아이템 수량 감소
	{
		shared_ptr<Button> minus = make_shared<Button>();
		minus->SetPos({ _pos.x + _size.x / 2 - 34, _pos.y + _size.y - 65 });
		minus->SetSize({ 30,18 });
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Default);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Hovered);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Clicked);
		minus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountMinusButton);
		minus->SetInitialPos(minus->GetPos());
		AddChild(minus);
	}

	// 아이템 수량 10개 감소
	{
		shared_ptr<Button> doubleMinus = make_shared<Button>();
		doubleMinus->SetPos({ _pos.x + _size.x / 2 - 78, _pos.y + _size.y - 65 });
		doubleMinus->SetSize({ 30,18 });
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Default);
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Hovered);
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Clicked);
		doubleMinus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountDoubleMinusButton);
		doubleMinus->SetInitialPos(doubleMinus->GetPos());
		AddChild(doubleMinus);
	}

	// 아이템 수량 증가
	{
		shared_ptr<Button> plus = make_shared<Button>();
		plus->SetPos({ _pos.x + _size.x / 2 + 42, _pos.y + _size.y - 65 });
		plus->SetSize({ 30,18 });
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Default);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Hovered);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Clicked);
		plus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountPlusButton);
		plus->SetInitialPos(plus->GetPos());
		AddChild(plus);
	}

	// 아이템 수량 10개 증가
	{
		shared_ptr<Button> doublePlus = make_shared<Button>();
		doublePlus->SetPos({ _pos.x + _size.x / 2 + 86, _pos.y + _size.y - 65 });
		doublePlus->SetSize({ 30,18 });
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Default);
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Hovered);
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Clicked);
		doublePlus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountDoublePlusButton);
		doublePlus->SetInitialPos(doublePlus->GetPos());
		AddChild(doublePlus);
	}
}

void ItemCountsPopUp::Tick()
{
	Super::Tick();

	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	Panel::DragAndMove(_rect);

	_totalPrice = _price * _counts;
}

void ItemCountsPopUp::Render(HDC hdc)
{
	if (_visible == false)
		return;

	// 배경
	::TransparentBlt(hdc,
		(int32)_pos.x,
		(int32)_pos.y,
		GetSize().x + 10,
		GetSize().y,
		_background->GetDC(),
		_background->GetPos().x,
		_background->GetPos().y,
		_background->GetSize().x,
		_background->GetSize().y,
		_background->GetTransparent());

	// 경고 아이콘
	::TransparentBlt(hdc,
		_pos.x + 10,
		_pos.y + (int)_size.y / 6,
		43,
		43,
		_icon->GetDC(),
		_icon->GetPos().x,
		_icon->GetPos().y,
		_icon->GetSize().x,
		_icon->GetSize().y,
		_icon->GetTransparent());

	// 아이템 갯수
	{
		wstring itemCount = to_wstring(_counts) + L"개";
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) + 15,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCount.c_str(), -1, &_textRect, DT_CENTER);
	}

	// 골드 아이콘
	{
		shared_ptr<Sprite> goldImage = GET_SINGLE(ResourceManager)->GetSprite(L"Gold");

		if (goldImage)
		::TransparentBlt(hdc,
			_pos.x + 115,
			_pos.y + 75,
			goldImage->GetSize().x,
			goldImage->GetSize().y,
			goldImage->GetDC(),
			0,
			0,
			goldImage->GetSize().x,
			goldImage->GetSize().y,
			goldImage->GetTransparent());
	}
	// 최종 비용
	{
		wstring totalPrice = to_wstring(_totalPrice);
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) - 10, _textRect.left + 30, _textRect.top + 18 };
		DrawTextW(hdc, totalPrice.c_str(), -1, &_textRect, DT_CENTER);
	}

	Super::Render(hdc);
}

shared_ptr<AlertBox> ItemCountsPopUp::MakeAlertBox(Vec2 pos, Vec2Int size, void(ItemCountsPopUp::* func)(), bool twoButtons)
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

	auto panel = dynamic_pointer_cast<Panel>(_parent);
	if (panel)
	{
		panel->AddChild(alert);
	}

	return alert;
}

void ItemCountsPopUp::SetIcon(wstring wstr)
{
	// Danger, Alert, Information
	if (wstr == L"Danger")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"DangerIcon");
	}
	else if (wstr == L"Alert")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"AlertIcon");
	}
	else
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
	}
}

void ItemCountsPopUp::OnClickAcceptButton()
{
	Super::OnClickAcceptButton();

	ResetPos();

	bool found = false;
	// 구매 확인
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (!myPlayer)
		return;

	if (_itemID <= 3)
		return;

	int gold = myPlayer->info.gold();
	int arrows = myPlayer->info.arrows();
	int maxHP = myPlayer->info.maxhp();
	int myPotion = myPlayer->GetPotionNums();
	int itemCounts = 0;
	wstring itemType = L"";

	auto item = GET_SINGLE(ItemManager)->GetItem(_itemID);
	if (item.ItemId > 0)
	{
		_price = item.Price;
		itemType = item.Type;
	}

	if (_price <= 0)
	{
		GET_SINGLE(ChatManager)->SendMessageToServer(format(L"Player {0} 잘못된 가격에 구입 시도.", myPlayer->GetObjectID()), false);
		return;
	}

	if (gold < _totalPrice)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"골드가 부족합니다.");
		return;
	}

	// 맥스 하트
	if (_itemID == 6)
	{
		if ((maxHP / 10) + _counts > 10)
			return;

		if (gold < _totalPrice)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + MAX_HP_AMOUNT * _counts, 0, 100));
		myPlayer->info.set_gold(myPlayer->info.gold() - _totalPrice);

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
		if (slot->ItemId == _itemID)
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
			if (itemType == L"Wearable")
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
			int maxCount = GET_SINGLE(ItemManager)->GetItem(_itemID).MaxCount;

			// 구입하려는 수량이 최대 수량을 초과할 때 리턴
			// 중첩 가능할 경우
			if (itemType != L"Wearable" && itemCounts + _counts > maxCount)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(format(L"아이템의 최대 보유 가능 수량 ({0})개를 초과했습니다.", maxCount));
				return;
			}

			// 갑옷 등 중첩이 불가능한 아이템 갯수가 남은 슬롯보다 많으면 리턴
			if (itemType == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(L"인벤토리에 빈 슬롯이 부족합니다.");
				return;
			}
		}
	}

	if (GET_SINGLE(ItemManager)->AddItemToInventory(_itemID, _counts) == false)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"아이템을 구매하지 못했습니다.");
		return;
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _totalPrice);

	// 골드 소모 정보 전송 - objectinfo 내에 골드 정보가 저장되어있음
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	SetPause(false);
}

void ItemCountsPopUp::OnClickDenyButton()
{
	Super::OnClickDenyButton();

	SetVisible(false);

	ResetPos();
}

void ItemCountsPopUp::OnClickCountPlusButton()
{
	_counts++;

	if (_counts > _maxCounts)
		_counts = _maxCounts;
}

void ItemCountsPopUp::OnClickCountDoublePlusButton()
{
	_counts += 10;

	if (_counts > _maxCounts)
		_counts = _maxCounts;
}

void ItemCountsPopUp::OnClickCountMinusButton()
{
	_counts--;

	if (_counts <= 1)
		_counts = 1;
}

void ItemCountsPopUp::OnClickCountDoubleMinusButton()
{
	_counts -= 10;

	if (_counts <= 1)
		_counts = 1;
}

void ItemCountsPopUp::SetText(wstring wstr)
{
	auto textbox = FindChild<TextBox>(_children);
	
	if (textbox)
		textbox->SetText(wstr);
}
