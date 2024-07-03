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

	// ����� ��ġ�� �߾ӿ� ��ġ��Ű�� ���ؼ��� Size ������ Pos �������� ����!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };
	_initialPos = _pos;
	// �ؽ�Ʈ
	{
		shared_ptr<TextBox> text = make_shared<TextBox>();
		text->SetText(L"�׽�Ʈ�� �ؽ�Ʈ�Դϴ�. \n�ι�° ���Դϴ�. \n ����° ���Դϴ�.");
		text->SetPos(Vec2{ _pos.x + 60, _pos.y + 20 });
		text->SetSize(Vec2Int{ _size.x - 80, (_size.y / 2) - 25 });
		text->SetPadding(3, 3);
		text->SetInitialPos(text->GetPos());
		AddChild(text);
	}

	// ������ ���� ����
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

	// ������ ���� 10�� ����
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

	// ������ ���� ����
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

	// ������ ���� 10�� ����
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

	// ���
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

	// ��� ������
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

	// ������ ����
	{
		wstring itemCount = to_wstring(_counts) + L"��";
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) + 15,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCount.c_str(), -1, &_textRect, DT_CENTER);
	}

	// ��� ������
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
	// ���� ���
	{
		wstring totalPrice = to_wstring(_totalPrice);
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) - 10, _textRect.left + 30, _textRect.top + 18 };
		DrawTextW(hdc, totalPrice.c_str(), -1, &_textRect, DT_CENTER);
	}

	Super::Render(hdc);
}

shared_ptr<AlertBox> ItemCountsPopUp::MakeAlertBox(Vec2 pos, Vec2Int size, void(ItemCountsPopUp::* func)(), bool twoButtons)
{
	// �˾�
	shared_ptr<AlertBox> alert = make_shared<AlertBox>();
	if (alert)
	{
		// AlertBox �ʱ�ȭ
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
	// ���� Ȯ��
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
		GET_SINGLE(ChatManager)->SendMessageToServer(format(L"Player {0} �߸��� ���ݿ� ���� �õ�.", myPlayer->GetObjectID()), false);
		return;
	}

	if (gold < _totalPrice)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"��尡 �����մϴ�.");
		return;
	}

	// �ƽ� ��Ʈ
	if (_itemID == 6)
	{
		if ((maxHP / 10) + _counts > 10)
			return;

		if (gold < _totalPrice)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + MAX_HP_AMOUNT * _counts, 0, 100));
		myPlayer->info.set_gold(myPlayer->info.gold() - _totalPrice);

		// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			return;
		}
	}

	for (auto& slot : inventory->GetSlots())
	{
		// �����Ϸ��� �������� �κ��丮�� �̹� �ִ��� Ȯ��
		if (slot->ItemId == _itemID)
		{
			found = true;
			itemCounts = slot->ItemCount;
			break;
		}
	}

	// IsInventoryFull�� ����Ͽ� ��� ������ ��á���� Ȯ��
	if (GET_SINGLE(ItemManager)->IsInventoryFull() == true)
	{
		// �κ��丮�� �̹� �ִٸ�
		if (found)
		{
			// ��ø�� �Ұ����� Ÿ��
			if (itemType == L"Wearable")
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(L"�κ��丮�� ���� á���ϴ�.");
				return;
			}
			// �κ��丮�� �� á����, ��ø�� ������ Ÿ�� -> if�� Ż��
		}
		else
		{
			auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
			alert->SetIcon(L"Warning");
			alert->SetText(L"�κ��丮�� ���� á���ϴ�.");
			return;	// �κ��丮�� �� á�µ� �ش� �������� ���Կ� ���ٸ� ����
		}
	}
	else // �κ��丮�� ���� ������ �ʾ��� ��
	{
		if (found)
		{
			int maxCount = GET_SINGLE(ItemManager)->GetItem(_itemID).MaxCount;

			// �����Ϸ��� ������ �ִ� ������ �ʰ��� �� ����
			// ��ø ������ ���
			if (itemType != L"Wearable" && itemCounts + _counts > maxCount)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(format(L"�������� �ִ� ���� ���� ���� ({0})���� �ʰ��߽��ϴ�.", maxCount));
				return;
			}

			// ���� �� ��ø�� �Ұ����� ������ ������ ���� ���Ժ��� ������ ����
			if (itemType == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
			{
				auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
				alert->SetIcon(L"Warning");
				alert->SetText(L"�κ��丮�� �� ������ �����մϴ�.");
				return;
			}
		}
	}

	if (GET_SINGLE(ItemManager)->AddItemToInventory(_itemID, _counts) == false)
	{
		auto alert = MakeAlertBox({ GWinSizeX / 2, GWinSizeY / 2 }, { 300, 150 }, nullptr, false);
		alert->SetIcon(L"Warning");
		alert->SetText(L"�������� �������� ���߽��ϴ�.");
		return;
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _totalPrice);

	// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
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
