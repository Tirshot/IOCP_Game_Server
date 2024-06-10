#include "pch.h"
#include "SliderBar.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "InputManager.h"

SliderBar::SliderBar()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"SliderBackground");
}

SliderBar::~SliderBar()
{
}

void SliderBar::BeginPlay()
{
	SetSize({ 150,16 });
	_percentage = (_value - _minimum) / (_maximum - _minimum);
}

void SliderBar::Tick()
{
	/// background size
	int sizeX = _background->GetSize().x;
	int sizeY = _background->GetSize().y;

	_rect.left = _pos.x - (sizeX / 2);
	_rect.top = _pos.y - (sizeY / 2);
	_rect.right = _rect.left + _size.x;
	_rect.bottom = _rect.top + _size.y;

	_value = ((float)(_maximum - _minimum) * _percentage) + _minimum;

	if (IsMouseInRect(_rect))
	{
		if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse))
		{
			auto mousePos = GET_SINGLE(InputManager)->GetMousePos();

			// 누른 지점
			_percentage = (float)(mousePos.x - _rect.left) / _size.x;

			if (_callbackFunc)
				_callbackFunc();
		}
	}
}

void SliderBar::Render(HDC hdc)
{
	/// background size
	int sizeX = _background->GetSize().x;
	int sizeY = _background->GetSize().y;

	// 뒷 배경
	::StretchBlt(hdc,
		_pos.x - (sizeX / 2),
		_pos.y - (sizeY / 2),
		_size.x,
		_size.y - 5,
		_background->GetDC(),
		0,
		0,
		sizeX,
		sizeY,
		SRCCOPY);

	// Fill 배경
	::StretchBlt(hdc,
		_pos.x - (sizeX / 2),
		_pos.y - (sizeY / 2),
		_size.x * _percentage,
		_size.y - 5,
		_background->GetDC(),
		0,
		0,
		_size.x * _percentage,
		sizeY,
		WHITENESS);
}
