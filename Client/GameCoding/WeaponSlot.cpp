#include "pch.h"
#include "WeaponSlot.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"

WeaponSlot::WeaponSlot()
{
	_slotBackground = GET_SINGLE(ResourceManager)->GetSprite(L"QuickSlot");
	_slotSelected = GET_SINGLE(ResourceManager)->GetSprite(L"Selected");
}

WeaponSlot::~WeaponSlot()
{

}

void WeaponSlot::BeginPlay()
{
	
}

void WeaponSlot::Tick()
{

}

void WeaponSlot::Render(HDC hdc)
{
	auto iconSize = GET_SINGLE(ResourceManager)->GetSprite(L"SwordItem")->GetSize().x;
	int size_x = _slotBackground->GetSize().x;
	int size_y = _slotBackground->GetSize().y;
	//auto sizeX = _size.x;

	// 무기 슬롯 배경
	::TransparentBlt(hdc,
		0,
		0,
		size_x,
		size_y,
		_slotBackground->GetDC(),
		_slotBackground->GetPos().x,
		_slotBackground->GetPos().y,
		size_x,
		size_y,
		_slotBackground->GetTransparent());

	::TransparentBlt(hdc,
		(int)(_pos.x + 60 * (0 - 1 / 2.f) + 4),
		(int)_pos.y - 60 + 7,
		45,
		45,
		_slotSelected->GetDC(),
		_slotSelected->GetPos().x,
		_slotSelected->GetPos().y,
		80,
		80,
		RGB(128, 128, 128));
}

//int16 WeaponSlot::HighlightSlot()
//{
//	/* 0, 1, 2*/
//	MyPlayer* player = GET_SINGLE(SceneManager)->GetMyPlayer();
//	if (player)
//		return player->GetSelectedSlot();
//
//	return 0; 
//}
