#include "pch.h"
#include "MerchantTutorial.h"
#include "Sprite.h"
#include "ResourceManager.h"

MerchantTutorial::MerchantTutorial()
{
	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"MerchantTutorial");
}

MerchantTutorial::~MerchantTutorial()
{
}

void MerchantTutorial::BeginPlay()
{
}

void MerchantTutorial::Render(HDC hdc)
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
