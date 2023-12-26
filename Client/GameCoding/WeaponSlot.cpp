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
	Sprite* weaponSword = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
	Sprite* weaponBow = GET_SINGLE(ResourceManager)->GetSprite(L"Bow");
	Sprite* weaponStaff = GET_SINGLE(ResourceManager)->GetSprite(L"Staff");
	_woodenSlot = GET_SINGLE(ResourceManager)->GetSprite(L"Slot");
	_selectedSlot = GET_SINGLE(ResourceManager)->GetSprite(L"SelectedSlot");

	// _slots�� push_back �ϸ� ������ �ڵ����� ����
	_slots.insert({ 1,weaponSword });
	_slots.insert({ 2,weaponBow });
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
		// ���� ���� ���
		::BitBlt(hdc,
			(int)(_pos.x + 60.f * ((i - 1) - _slots.size() / 2.f)),
			(int)_pos.y - 60,
			60,
			60,
			_woodenSlot->GetDC(),
			_woodenSlot->GetPos().x,
			_woodenSlot->GetPos().y,
			SRCCOPY);

		// ���� ������
		::TransparentBlt(hdc,
			(int)(_pos.x + 60 * ((i - 1)  - _slots.size() / 2.f) + 7),
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

	// ���õ� ���� ���̶���Ʈ
	::TransparentBlt(hdc,
		(int)(_pos.x + 60 * (HighlightSlot() - _slots.size() / 2.f) + 6),
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
		return 0;

	return player->GetWeaponType(); /* 0, 1, 2*/
}
