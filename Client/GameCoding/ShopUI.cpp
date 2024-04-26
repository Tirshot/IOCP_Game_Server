#include "pch.h"
#include "ShopUI.h"
#include "Texture.h"
#include "Button.h"
#include "TextBox.h"
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
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + 450, _pos.y + 360 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &ShopUI::OnClickBackButton);
		AddChild(back);
	}

	{ // ��ȭ ����
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
	{ // ����
		AddSellItem(5);
		int index = GetItemIndex(5);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(5);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // ����00
		AddSellItem(12);
		int index = GetItemIndex(12);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(12);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}
	{ // ����01
		AddSellItem(13);
		int index = GetItemIndex(13);
		ITEM* itemInfo = &GET_SINGLE(ItemManager)->GetItem(13);
		ShopItemPanel* itemPanel = new ShopItemPanel(itemInfo, index, _pos);
		AddChild(itemPanel);
	}

	// ������ ���� Ȯ�� â
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

	for (auto& child : _children)
		child->BeginPlay();
}

void ShopUI::Tick()
{
	Panel::DragAndMove(&_rect);

	if (_visible)
	{
		for (auto& child : _children)
		{
			child->Tick();

			auto itemPanel = dynamic_cast<ShopItemPanel*>(child);

			if (itemPanel == nullptr)
				continue;

			if (IsMouseInRect(itemPanel->GetRect()))
			{
				if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
				{
					_sellItem = itemPanel->GetItem();
					_price = _sellItem->Price;
					_countsPopUp->SetText(_sellItem->KorName + L"��(��) �� �� �����Ͻðڽ��ϱ�?");
					_countsPopUp->SetVisible(true);
				}
			}
		}
		_counts = _countsPopUp->GetCounts();
		_allCost = _counts * _price;
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

void ShopUI::OnPopClickAcceptDelegate()
{
	// ���� Ȯ��
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
		return;

	// IsInventoryFull�� ����Ͽ� ��� ������ ��á���� Ȯ��
	if (GET_SINGLE(ItemManager)->IsInventoryFull() == true)
	{
		// �����Ϸ��� �������� �κ��丮�� �̹� �ִ��� Ȯ��
		for (auto& slot : inventory->GetSlots())
		{
			if (slot.ItemId == _sellItem->ItemId)
			{
				//// �̹� �����Ѵٸ� ��ø�� ������ Ÿ������ Ȯ��
				////// ���� �� ��ø�� �Ұ����� �������̶�� ����
				if (slot.Type == L"Wearable")
				{
					GET_SINGLE(ChatManager)->AddMessage(L"�κ��丮�� ���� á���ϴ�.");
					return;
				}
				else
				{
					////// �Ҹ�ǰ �� ��ø�� ������ �������̶�� ��ø
					// �ƽ� ��Ʈ
					if (_sellItem->ItemId == 6)
					{
						if (maxHP + _counts > 10)
							return;

						myPlayer->info.set_maxhp(clamp(maxHP + _counts, 0, 10));
					}

					myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

					// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
					{
						SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
						GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
					}
				}
				GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts);
				return;
			}
		}
	}
	else
	{
		// ���� �κ��丮�� ���� ���� �ʾ���
		//// �������� �߰��ϸ� �κ��丮�� ���� ���� �Ǵ��� Ȯ��
		// �����Ϸ��� �������� �κ��丮�� �̹� �ִ��� Ȯ��
		for (auto& slot : inventory->GetSlots())
		{
			if (slot.ItemId == _sellItem->ItemId)
			{
				//// �̹� �����Ѵٸ� ��ø�� ������ Ÿ������ Ȯ��
				////// ���� �� ��ø�� �Ұ����� ������ ������ ���� ���Ժ��� ������ ����
				if (slot.Type == L"Wearable" && GET_SINGLE(ItemManager)->GetEmptySlots() < _counts)
				{
					GET_SINGLE(ChatManager)->AddMessage(L"�κ��丮�� ���� á���ϴ�.");
					return;
				}
				else
				{
					////// �Ҹ�ǰ �� ��ø�� ������ �������̶�� ��ø
					// �ƽ� ��Ʈ
					if (_sellItem->ItemId == 6)
					{
						if (maxHP + _counts > 10)
							return;

						myPlayer->info.set_maxhp(clamp(maxHP + _counts, 0, 10));
					}

					myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

					// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
					{
						SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
						GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
					}
				}
				GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts);
				return;
			}
		}

	}
	
	// �ƽ� ��Ʈ
	if (_sellItem->ItemId == 6)
	{
		if (maxHP + _counts > 10)
			return;

		myPlayer->info.set_maxhp(clamp(maxHP + _counts, 0, 10));
	}

	myPlayer->info.set_gold(myPlayer->info.gold() - _allCost);

	// ��� �Ҹ� ���� ���� - objectinfo ���� ��� ������ ����Ǿ�����
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}

	GET_SINGLE(ItemManager)->AddItemToInventory(_sellItem->ItemId, _counts);
}

void ShopUI::OnClickBackButton()
{
	SetVisible(false);
	SetChildVisible(false);
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);
}

void ShopUI::OnClickExitButton()
{
	SetVisible(false);
	SetChildVisible(false);

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

		// ��ư�� �θ��� visiblity�� ������ ����
		auto button = dynamic_cast<Button*>(child);

		if (button)
			button->SetVisible(true);
	}
}
