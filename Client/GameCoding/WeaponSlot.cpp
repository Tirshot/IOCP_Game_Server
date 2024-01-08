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

}

void WeaponSlot::BeginPlay()
{
	Sprite* weaponSword = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
	Sprite* weaponBow = GET_SINGLE(ResourceManager)->GetSprite(L"Bow");
	Sprite* weaponStaff = GET_SINGLE(ResourceManager)->GetSprite(L"Staff");
	_woodenSlot = GET_SINGLE(ResourceManager)->GetSprite(L"Frame");
	_selectedSlot = GET_SINGLE(ResourceManager)->GetSprite(L"SelectedSlot");
	_selected = GET_SINGLE(ResourceManager)->GetSprite(L"Selected");

	// _slots에 push_back 하면 슬롯이 자동으로 증가
	_slots.insert({ 1, weaponSword });
	_slots.insert({ 2, weaponBow });
	_slots.insert({ 3, weaponStaff });
}

void WeaponSlot::Tick()
{

}

void WeaponSlot::Render(HDC hdc)
{
	auto iconSize = GET_SINGLE(ResourceManager)->GetSprite(L"Sword")->GetSize().x;

	auto sizeX = _size.x;
	// 52 * 52

	for (int i = 1; i <= min(_slots.size(), MAX_SLOT); i++)
	{
		// 무기 슬롯 배경
		::TransparentBlt(hdc,
			(int)(_pos.x + 60.f * ((i - 1) - _slots.size() / 2.f) + 1),
			(int)_pos.y - 56,
			52,
			52,
			_woodenSlot->GetDC(),
			_woodenSlot->GetPos().x,
			_woodenSlot->GetPos().y,
			74,
			74,
			RGB(128, 128, 128));

		// 무기 아이콘
		::TransparentBlt(hdc,
			(int)(_pos.x + 60 * ((i - 1)  - _slots.size() / 2.f) + 5),
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

	::TransparentBlt(hdc,
		(int)(_pos.x + 60 * (HighlightSlot() - _slots.size() / 2.f) + 4),
		(int)_pos.y - 60 + 7,
		45,
		45,
		_selected->GetDC(),
		_selected->GetPos().x,
		_selected->GetPos().y,
		80,
		80,
		RGB(128, 128, 128));
}

int16 WeaponSlot::HighlightSlot()
{
	Player* player = GET_SINGLE(SceneManager)->GetMyPlayer();

	return player ? player->GetWeaponType() : 0; /* 0, 1, 2*/
}
