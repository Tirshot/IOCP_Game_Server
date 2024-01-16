#include "pch.h"
#include "NamePlate.h"
#include "DevScene.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

NamePlate::NamePlate()
{
	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");

	_rect.left = _pos.x - _size.x / 2;
	_rect.right = _rect.left + 130;
	_rect.top = _pos.y - _size.y / 2;
	_rect.bottom = _rect.top + 28;

	SetSize({ 130,28 });
}

NamePlate::NamePlate(wstring wstr)
{
	_name = wstr;

	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");

	SetSize({ 130,28 });
}

NamePlate::~NamePlate()
{
}

void NamePlate::BeginPlay()
{

}

void NamePlate::Tick()
{

}

void NamePlate::Render(HDC hdc)
{
	::BitBlt(hdc,
		_pos.x - _size.x / 2,
		_pos.y + _size.y / 2,
		130,
		28,
		_sprite->GetDC(),
		0,
		0,
		SRCCOPY);

	_rect.left = _pos.x - _size.x / 2;
	_rect.right = _rect.left + _size.x;
	_rect.top = _pos.y + _size.y / 2 + 5;
	_rect.bottom = _rect.top + _size.y;

	SetTextColor(hdc, RGB(255, 255, 255));
	::DrawTextW(hdc, _name.c_str(), -1, &_rect, DT_CENTER || DT_VCENTER || DT_SINGLELINE);
}
