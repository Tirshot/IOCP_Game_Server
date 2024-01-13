#include "pch.h"
#include "Gold.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

Gold::Gold()
{
	_pos = { 10, 60 };
	_golds = GET_SINGLE(ResourceManager)->GetSprite(L"Gold");
	_rect.left = _pos.x + _size.x + 5;
	_rect.right = _pos.x + 100;
	_rect.top = 65;
	_rect.bottom = 80;
}

Gold::~Gold()
{

}

void Gold::BeginPlay()
{

}

void Gold::Tick()
{
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (_myPlayer)
		_gold = _myPlayer->info.gold();
}

void Gold::Render(HDC hdc)
{
	// °ñµå ½ºÇÁ¶óÀÌÆ®
	::TransparentBlt(hdc,
		_pos.x,
		_pos.y,
		23,
		24,
		_golds->GetDC(),
		0,
		0,
		23,
		24,
		_golds->GetTransparent());

	// °ñµå °¹¼ö ÅØ½ºÆ®
	SetTextColor(hdc, RGB(0, 0, 0));
	wstring str = std::format(L"{0}", _gold);
	::DrawTextW(hdc, str.c_str(), -1, &_rect, DT_LEFT);
}
