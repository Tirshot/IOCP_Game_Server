#include "pch.h"
#include "MiniMap.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"

MiniMap::MiniMap()
{
	_map = GET_SINGLE(ResourceManager)->GetSprite(L"Stage01_mini");
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");

}

MiniMap::~MiniMap()
{
}

void MiniMap::BeginPlay()
{

}

void MiniMap::Tick()
{
}

void MiniMap::Render(HDC hdc)
{
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer == nullptr)
		return;

	auto pos = myPlayer->GetPos();
	auto cellPos = myPlayer->GetCellPos();

	::StretchBlt(hdc,
		_pos.x - 3,
		_pos.y,
		_size.x + 3,
		_size.y + 23,
		_background->GetDC(),
		0,
		0,
		100,
		100,
		SRCCOPY);

	::StretchBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_map->GetDC(),
		pos.x / _scale - _map->GetSize().x / (_scale * _scale),
		pos.y / _scale - _map->GetSize().y / (_scale * _scale),
		_map->GetSize().x / _scale,
		_map->GetSize().y / _scale,
		SRCCOPY);

	::BitBlt(hdc,
		_pos.x + _size.x / 2,
		_pos.y + _size.y / 2,
		5,
		5,
		_background->GetDC(),
		5,
		5,
		WHITENESS);

	wstring _text = ::format(L"({0}, {1})", (int)cellPos.x, (int)cellPos.y);
	RECT _rect = {};
	_rect.left = _pos.x;
	_rect.top = _pos.y + _size.y + 2;
	_rect.right = _rect.left + _size.x;
	_rect.bottom = _pos.y + _size.y + 20;

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	::DrawTextW(hdc, _text.c_str(), -1, &_rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
}
