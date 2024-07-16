#include "pch.h"
#include "ArrowUI.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

ArrowUI::ArrowUI()
{
	_pos = { 90, 60 };
	_size = { 24, 23 };
	_arrows = GET_SINGLE(ResourceManager)->GetSprite(L"ArrowUI");
	_rect.left = _pos.x + _size.x + 3;
	_rect.right = _pos.x + 100;
	_rect.top = 65;
	_rect.bottom = 80;
}

ArrowUI::~ArrowUI()
{
	_myPlayer = nullptr;
	_arrows = nullptr;
}

void ArrowUI::BeginPlay()
{
	_arrowsCount = 10;
}

void ArrowUI::Tick()
{
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (_myPlayer)
		_arrowsCount = _myPlayer->info.arrows();
}

void ArrowUI::Render(HDC hdc)
{
	// 화살 스프라이트
	::TransparentBlt(hdc,
		_pos.x,
		_pos.y,
		24,
		23,
		_arrows->GetDC(),
		0,
		0,
		24,
		23,
		_arrows->GetTransparent());

	// 골드 갯수 텍스트
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	wstring str = std::format(L"{0}", _arrowsCount);
	::DrawTextW(hdc, str.c_str(), -1, &_rect, DT_LEFT);
}
