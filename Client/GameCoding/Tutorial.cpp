#include "pch.h"
#include "Tutorial.h"
#include "Sprite.h"
#include "ResourceManager.h"

Tutorial::Tutorial()
{
	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Tutorial");
}

Tutorial::~Tutorial()
{
}

void Tutorial::BeginPlay()
{
}

void Tutorial::Render(HDC hdc)
{
	::BitBlt(hdc,
		_pos.x,
		_pos.y,
		400,
		300,
		_sprite->GetDC(),
		0,
		0,
		SRCCOPY);
}
