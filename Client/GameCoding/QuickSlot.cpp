#include "pch.h"
#include "QuickSlot.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ItemManager.h"

QuickSlot::~QuickSlot()
{
	_slots.clear();

	_slotBackground = nullptr;
	_slotSelectedSprite = nullptr;
	_selectedItem = nullptr;
}

void QuickSlot::BeginPlay()
{
	_slotBackground = GET_SINGLE(ResourceManager)->GetSprite(L"QuickSlot");
	_slotSelectedSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Selected");

	_size_x = _slotBackground->GetSize().x;
	_size_y = _slotBackground->GetSize().y;

	// ������ �߾� �ϴܿ� ��ġ
	_center_x = _pos.x - (_size_x/ 2);
	_center_y = _pos.y - (_size_y);

	_slots.assign( 9, {0});
	_slots[1] = GET_SINGLE(ItemManager)->GetItem(1);
	_slots[2] = GET_SINGLE(ItemManager)->GetItem(2);
	_slots[3] = GET_SINGLE(ItemManager)->GetItem(3);

	_pressedButton = 1;
}

void QuickSlot::Tick()
{
	if (GET_SINGLE(InputManager)->IsPressedAnyNumberKey())
	{
		// ���õ� ���� ���̶���Ʈ
		SetPressedButton();
	}
}

void QuickSlot::Render(HDC hdc)
{
	// ���� ���� ���
	::TransparentBlt(hdc,
		_center_x,
		_center_y,
		_size_x,
		_size_y,
		_slotBackground->GetDC(),
		0,
		0,
		_size_x,
		_size_y,
		_slotBackground->GetTransparent());

	// ���õ� ���� ���̶���Ʈ
	::TransparentBlt(hdc,
		_pos.x - _slotSize * 5 + _slotSize * (_selectedIndex),
		_pos.y - 65,
		_slotSize,
		_slotSize,
		_slotSelectedSprite->GetDC(),
		_slotSelectedSprite->GetPos().x,
		_slotSelectedSprite->GetPos().y,
		80,
		80,
		RGB(128, 128, 128));

	// �� ���Կ� ��ϵ� ������
	for (int i = 1; i < 9; i++)
	{
		if (_slots[i].Sprite == nullptr)
			continue;

		auto posX = _center_x + (_slotSize / 6) + _slotSize * (i - 1);
		auto posY = _center_y + _slotSize / 6;

		::TransparentBlt(hdc,
			posX,
			posY,
			_slotSize / 1.5f,
			_slotSize / 1.5f,
			_slots[i].Sprite->GetDC(),
			_slots[i].Sprite->GetPos().x,
			_slots[i].Sprite->GetPos().y,
			_slots[i].Sprite->GetSize().x,
			_slots[i].Sprite->GetSize().y,
			_slots[i].Sprite->GetTransparent());

		// �Ҹ�ǰ ���� ǥ��
		if (_slots[i].Type == L"Consumable")
		{
			auto item = GET_SINGLE(ItemManager)->FindItemFromInventory(_slots[i].ItemId);
			if (item == nullptr)
				return;

			int itemCount = item->ItemCount;

			wstring str = to_wstring(itemCount);
			RECT textRect = {};
			textRect.left = posX + (_slotSize / 2);
			textRect.right = textRect.left + 20;
			textRect.top = posY - 5;
			textRect.bottom = textRect.top + 20;
			::DrawTextW(hdc, str.c_str(), -1, &textRect, DT_LEFT);
		}
	}
}

void QuickSlot::SetQuickSlot(shared_ptr<ITEM> item, int index)
{
	if (index < 1 || index > 9)
		return;

	// �� ĭ�� ��� �Ұ�
	if (item->ItemId <= 0)
		return;

	// ���� ���� �������� ��� �Ұ�
	if (item->Type == L"ETC")
		return;

	// ȭ���� ��� �Ұ�
	if (item->SubType == L"Arrow")
		return;

	// ������ �������� ����ϸ� ��� ���
	if (_slots[index].ItemId == item->ItemId)
	{
		_slots[index] = {};
	}

	// ������ �������� �ٸ� ���Կ� ����ϸ� ���� ��� ���
	for (auto& slot : _slots)
	{
		if (slot.ItemId == item->ItemId)
		{
			slot = {};
			break;
		}
	}
	_slots[index] = *item;
}

void QuickSlot::SetPressedButton()
{
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer)
	{
		if (myPlayer->GetState() != IDLE && myPlayer->GetState() != MOVE)
			return;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1))
	{
		_pressedButton = 1;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		_pressedButton = 2;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3))
	{
		_pressedButton = 3;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_4))
	{
		_pressedButton = 4;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_5))
	{
		_pressedButton = 5;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_6))
	{
		_pressedButton = 6;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_7))
	{
		_pressedButton = 7;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_8))
	{
		_pressedButton = 8;
	}

	int temp = _selectedIndex;

	if (_selectedIndex == _pressedButton)
	{
		// �Ҹ�ǰ�̸� ��� ���
		if (_slots[_selectedIndex].Type == L"Consumable")
		{
			UseConsumableItem();
		}
	}
	
	_selectedIndex = _pressedButton;

	// ��ȿ���� ���� ������ ���� �Ұ�
	if (_slots[_selectedIndex].ItemId != 0)
	{
		GET_SINGLE(ItemManager)->QuickEquipItem(_slots[_selectedIndex].ItemId);
	}

	_pressedButton = 0;
}

void QuickSlot::UseConsumableItem()
{
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	if (myPlayer == nullptr)
		return;

	auto& slot = _slots[_selectedIndex];
	int itemID = slot.ItemId;

	myPlayer->UseConsumableItem(itemID);
}
