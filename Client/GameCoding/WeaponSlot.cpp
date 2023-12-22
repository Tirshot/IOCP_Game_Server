#include "pch.h"
#include "WeaponSlot.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"

WeaponSlot::WeaponSlot()
{

}

WeaponSlot::~WeaponSlot()
{
	RemoveChild(this);
}

void WeaponSlot::BeginPlay()
{
	AddChild(this);
	auto weaponSword = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
	auto weaponBow = GET_SINGLE(ResourceManager)->GetSprite(L"Bow");
	auto weaponStaff = GET_SINGLE(ResourceManager)->GetSprite(L"Staff");
	_woodenSlot = GET_SINGLE(ResourceManager)->GetSprite(L"Slot");
	_selectedSlot = GET_SINGLE(ResourceManager)->GetSprite(L"SelectedSlot");

	_slots.push_back(weaponSword);
	_slots.push_back(weaponBow);
	_slots.push_back(weaponStaff);
}

void WeaponSlot::Tick()
{

}

void WeaponSlot::Render(HDC hdc)
{
	auto iconSize = GET_SINGLE(ResourceManager)->GetSprite(L"Sword")->GetSize().x;

	auto sizeX = _size.x;
	// 52 * 52

	for (int i = 0; i < _slots.size(); i++)
	{
		// 무기 슬롯 배경
		::BitBlt(hdc,
			(int)(_pos.x + 60.f * (i - _slots.size() / 2.f)),
			(int)_pos.y - 60,
			60,
			60,
			_woodenSlot->GetDC(),
			_woodenSlot->GetPos().x,
			_woodenSlot->GetPos().y,
			SRCCOPY);

		// 무기 아이콘
		::TransparentBlt(hdc,
			(int)(_pos.x + 60 * (i  - _slots.size() / 2.f) + 7),
			(int)_pos.y - 60 + 10,
			iconSize - 10,
			iconSize - 10,
			_slots[i]->GetDC(),
			_slots[i]->GetPos().x,
			_slots[i]->GetPos().y,
			iconSize,
			iconSize,
			_slots[i]->GetTransparent());
	}

	// 선택된 무기 하이라이트
	::TransparentBlt(hdc,
		(int)(_pos.x + 60 * (HighlightSlot() - 1 - (float)_slots.size() / 2) + 6),
		(int)_pos.y - 60 + 7,
		44,
		44,
		_selectedSlot->GetDC(),
		_selectedSlot->GetPos().x,
		_selectedSlot->GetPos().y,
		44,
		44,
		_selectedSlot->GetTransparent());
}

int16 WeaponSlot::HighlightSlot()
{
	Player* player = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (player == nullptr)
		return 1;

	auto nowWeapon = player->GetWeaponType();

	switch (nowWeapon)
	{
	case 0: /* Sword */
		return 1;

	case 1: /* Bow */
		return 2;

	case 2: /* Staff */
		return 3;
	}
}
