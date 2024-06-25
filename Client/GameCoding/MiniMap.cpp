#include "pch.h"
#include "MiniMap.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"
#include "DevScene.h"

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

	// 맵 그리기
	float width = _map->GetSize().x;
	float height = _map->GetSize().y;
	float scaleWidth = width / (_scale * _scale);
	float scaleHeight = height / (_scale * _scale);

	float srcX = (pos.x / _scale) - scaleWidth / 2;
	float srcY = (pos.y / _scale) - scaleHeight / 2;

	::StretchBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_map->GetDC(),
		srcX,
		srcY,
		scaleWidth,
		scaleHeight,
		SRCCOPY);

	auto scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	if (scene)
	{
		auto actors = scene->GetActors();
		if (actors.empty() == false)
		{
			for (const auto& actor : actors)
			{
				auto actorPos = actor->GetPos();
				auto Vec = actorPos - pos;

				int screenX = _pos.x + _size.x / 2.f + Vec.x / (_scale * 4);
				int screenY = _pos.y + _size.y / 2.f + Vec.y / (_scale * 4);

				// 컬링
				if (screenX < _pos.x  || screenX > GWinSizeX || screenY < 0 || screenY > _size.y)
					continue;

				::BitBlt(hdc,
					screenX,
					screenY,
					5,
					5,
					_background->GetDC(),
					5,
					5,
					WHITENESS);
			}
		}
	}

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
