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
}

void QuickSlot::BeginPlay()
{
	_slotBackground = GET_SINGLE(ResourceManager)->GetSprite(L"QuickSlot");
	_slotSelectedSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Selected");

	_size_x = _slotBackground->GetSize().x;
	_size_y = _slotBackground->GetSize().y;

	// 기즈모는 중앙 하단에 위치
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
	// 선택된 슬롯 하이라이트
	SetPressedButton();
}

void QuickSlot::Render(HDC hdc)
{
	// 무기 슬롯 배경
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

	// 선택된 슬롯 하이라이트
	::TransparentBlt(hdc,
		_pos.x - _slotSize * 5 + _slotSize * (_pressedButton),
		_pos.y - 65,
		_slotSize,
		_slotSize,
		_slotSelectedSprite->GetDC(),
		_slotSelectedSprite->GetPos().x,
		_slotSelectedSprite->GetPos().y,
		80,
		80,
		RGB(128, 128, 128));

	// 퀵 슬롯에 등록된 아이템
	for (int i = 1; i < 9; i++)
	{
		if (_slots[i].Sprite == nullptr)
			continue;

		::TransparentBlt(hdc,
			_center_x + (_slotSize / 6) + _slotSize * (i - 1),
			_center_y + _slotSize / 6,
			_slotSize / 1.5f,
			_slotSize / 1.5f,
			_slots[i].Sprite->GetDC(),
			_slots[i].Sprite->GetPos().x,
			_slots[i].Sprite->GetPos().y,
			_slots[i].Sprite->GetSize().x,
			_slots[i].Sprite->GetSize().y,
			_slots[i].Sprite->GetTransparent());
	}
}

void QuickSlot::SetQuickSlot(shared_ptr<ITEM> item, int index)
{
	if (index < 1 || index > 9)
		return;

	// 빈 칸은 등록 불가
	if (item->ItemId <= 0)
		return;

	// 몬스터 고유 아이템은 등록 불가
	if (item->Type == L"ETC")
		return;

	// 화살은 등록 불가
	if (item->SubType == L"Arrow")
		return;

	// 동일한 아이템을 등록하면 등록 취소
	if (_slots[index].ItemId == item->ItemId)
	{
		_slots[index] = {};
	}

	// 동일한 아이템을 다른 슬롯에 등록하면 기존 등록 취소
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

	_selectedIndex = _pressedButton;

	// 유효하지 않은 아이템 장착 불가
	if (_slots[_selectedIndex].ItemId != 0)
		GET_SINGLE(ItemManager)->QuickEquipItem(_slots[_selectedIndex].ItemId);
}
