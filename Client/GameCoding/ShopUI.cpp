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

	{ // �ڷ� ��ư
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

	{ // ��ȭ ����
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

	// ������ ����
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

	// ������ ����
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
		// ����
		AddSellItem(5);
		// ����00
		AddSellItem(12);
		// ����01
		AddSellItem(13);
		// ����01
		AddSellItem(14);
		// ����01
		AddSellItem(15);
		// ����01
		AddSellItem(16);
		// ����01
		AddSellItem(17);
		AddSellItem(18);
		AddSellItem(19);
		AddSellItem(20);
		AddSellItem(21);
		AddSellItem(22);
		AddSellItem(23);
		AddSellItem(25);
		AddSellItem(26);
		AddSellItem(1);
		AddSellItem(2);
		AddSellItem(3);
	}

	// ������ ���� Ȯ�� â
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
			// �˾�
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
			child->Tick();

		// �ڽ� �˾��� ���� �� �۵� ����
		if (IsChildPopUpVisible())
			return;

		if (IsAnyPopUpVisible())
			return;

		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_initializeTime += deltaTime;
		{
			_rect.left = _pos.x;
			_rect.top = _pos.y;
			_rect.right = _pos.x + 535;
			_rect.bottom = _pos.y + 450;
		}

		for (auto& child : _children)
		{
			// �Ǹ�â�� ������ ���� ǰ���� ������ ���� ����
			if (_initializeTime <= 0.5f)
				continue;

			auto Item = dynamic_pointer_cast<ShopItemPanel>(child);
			if (Item == nullptr)
				continue;

			RECT ItemRect = Item->GetRect();
			int index = Item->GetItemIndex();
			int page = 1 + index / 14;

			// �����ִ� �������� �������� �ִ� �������� �������� ������ �ǳʶ�
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
					_countsPopUp->SetText(_sellItem->KorName + L"��(��) �� �� �����Ͻðڽ��ϱ�?");
					_countsPopUp->SetPrice(_price);
					_countsPopUp->SetMaxCounts(99);
					_countsPopUp->SetVisible(true);
					_initializeTime = 0.f;
				}
			}
		}
	}
}

void ShopUI::Render(HDC hdc)
{
	// ���
	::BitBlt(hdc,
			_pos.x,
			_pos.y,
			_size.x,
			_size.y,
			_background->GetDC(),
			0,
			0,
			SRCCOPY);

	// ������ ����
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

			// �����ִ� �������� �������� �ִ� �������� �������� ������ �ǳʶ�
			if (_page != page)
				continue;
		}
		// �������� �����ϰų� ShopItemPanel�� �ƴϸ� ������
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
		_alert->SetText(L"�⺻ ����� �Ǹ��� �� �����ϴ�.");
		_alert->SetIcon(L"Alert");
	}
	else
	{
		_alert->SetText(item->KorName + L"��(��) ���� " + to_wstring(_sellToShop->Price / _buyPriceDivider) +L"��忡 �Ǹ��Ͻðڽ��ϱ� ? ");
		_alert->SetIcon(L"Warning");
	}

	_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
	_alert->SetVisible(true);
}

void ShopUI::OnPopClickAcceptDelegate()
{
	bool found = false;
	// ���� Ȯ��
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	if (!myPlayer)
		return;

	int gold = myPlayer->info.gold();
	int arrows = myPlayer->info.arrows();
	int maxHP = myPlayer->info.maxhp();
	int myPotion = myPlayer->GetPotionNums();

	if (gold < _allCost)
	{
		_alert->SetIcon(L"Warning");
		_alert->SetText(L"��尡 �����մϴ�.");
		_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
		_alert->SetVisible(true);
		return;
	}

	// �ƽ� ��Ʈ
	if (_sellItem->ItemId == 6)
	{
		if (maxHP + _counts > 10)
			return;

		if (gold < _allCost)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + _counts, 0, 10));
		myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

		// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			return;
		}
	}

	// IsInventoryFull�� ����Ͽ� ��� ������ ��á���� Ȯ��
	if (GET_SINGLE(ItemManager)->IsInventoryFull() == true)
	{
		for (auto& slot : inventory->GetSlots())
		{
			// �����Ϸ��� �������� �κ��丮�� �̹� �ִ��� Ȯ��
			if (slot->ItemId == _sellItem->ItemId)
			{
				found = true;
				break;
			}
		}

		// �κ��丮�� �̹� �ִٸ�
		if (found)
		{
			// ��ø�� �Ұ����� Ÿ��
			if (_sellItem->Type == L"Wearable")
			{
				_alert->SetIcon(L"Warning");
				_alert->SetText(L"�κ��丮�� ���� á���ϴ�.");
				_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
				_alert->SetVisible(true);
				return;
			}
			// �κ��丮�� �� á����, ��ø�� ������ Ÿ�� -> if�� Ż��
		}
		else
		{
			_alert->SetIcon(L"Warning");
			_alert->SetText(L"�κ��丮�� ���� á���ϴ�.");
			_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
			_alert->SetVisible(true);
			return;	// �κ��丮�� �� á�µ� �ش� �������� ���Կ� ���ٸ� ����
		}
	}
	else // �κ��丮�� ���� ������ �ʾ��� ��
	{
		// �������� �߰��ϸ� �κ��丮�� ���� ���� �Ǵ��� Ȯ��
		for (auto& slot : inventory->GetSlots())
		{
			// �����Ϸ��� �������� �κ��丮�� �̹� �ִ��� Ȯ��
			if (slot->ItemId == _sellItem->ItemId)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			// ���� �� ��ø�� �Ұ����� ������ ������ ���� ���Ժ��� ������ ����
			if (_sellItem->Type == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
			{
				_alert->SetIcon(L"Warning");
				_alert->SetText(L"�κ��丮�� �����մϴ�.");
				_alert->SetPos(Vec2{ GWinSizeX / 2, GWinSizeY / 2 });
				_alert->SetVisible(true);
				return;
			}
		}
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

	// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}

	GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts);
}

void ShopUI::OnPopClickAlertAcceptDelegate()
{
	if (_sellToShop == nullptr)
		return;

	int gold = _sellToShop->Price / _buyPriceDivider;
	int counts = _sellToShop->ItemCount;

	// ���� Ȯ��
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
		_countsPopUp->SetText(sellingItem->KorName + L"��(��) �� �� �Ǹ��Ͻðڽ��ϱ�?");
		_countsPopUp->SetPrice(gold);
		_countsPopUp->SetMaxCounts(counts);
		_countsPopUp->AddParentDelegate(this, &ShopUI::OnClickSellItemToShopAlertAcceptDelegate);
		_countsPopUp->SetVisible(true);
	}
}

void ShopUI::OnClickSellItemToShopAlertAcceptDelegate()
{
	int gold = _sellToShop->Price / 5;
	int counts = _sellToShop->ItemCount;

	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	auto inventory = GET_SINGLE(ItemManager)->GetInventory();

	auto sellingItem = inventory->FindItemFromInventory(_sellToShop);

	inventory->RemoveItem(sellingItem, counts);
	myPlayer->info.set_gold(myPlayer->info.gold() + (gold * counts));
	_sellToShop = nullptr;

	_countsPopUp->AddParentDelegate(this, &ShopUI::OnPopClickAcceptDelegate);
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

		// ��ư�� �θ��� visiblity�� ������ ����
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
