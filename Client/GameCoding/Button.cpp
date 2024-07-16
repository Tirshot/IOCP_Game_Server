#include "pch.h"
#include "Button.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Sprite.h"
#include "SoundManager.h"

Button::Button()
{
}

Button::~Button()
{
	_sprites->reset();
	_onClick = nullptr;
	_currentSprite.reset();
}

void Button::BeginPlay()
{
	SetButtonState(BS_Default);
}

void Button::Tick()
{
	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (_pause == true)
		return;

	if (_state == BS_Clicked)
	{
		_sumTime += deltaTime;
		if (_sumTime >= 0.2f)
		{
			_sumTime = 0.f;
			SetButtonState(BS_Default);
		}
	}

	if (IsMouseInRect())
	{
		SetCurrentSprite(_sprites[BS_Hovered]);

		// 눌림
		if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse))
		{
			SetButtonState(BS_Pressed);
		}
		else
		// 뗌
		{
			// 누르고 있다가 뗌
			if (_state == BS_Pressed)
			{
				SetButtonState(BS_Clicked);
				// OnClicked
				if (_onClick)
					_onClick();

				GET_SINGLE(SoundManager)->Play(L"Button");
			}
		}
	}
	else
	{
		// 마우스가 밖에 있음
		SetButtonState(BS_Default);
	}
}

void Button::Render(HDC hdc)
{
	if (_visible == true)
	{// 버튼에 스프라이트가 적용되어 있다면 그리기
		if (_currentSprite)
		{
			::TransparentBlt(hdc,
				(int32)_pos.x - _size.x / 2,
				(int32)_pos.y - _size.y / 2,
				_size.x,
				_size.y,
				_currentSprite->GetDC(),
				_currentSprite->GetPos().x,
				_currentSprite->GetPos().y,
				_currentSprite->GetSize().x,
				_currentSprite->GetSize().y,
				RGB(128,128,128));
		}
	}
}

void Button::SetButtonState(ButtonState state)
{
	_state = state;

	if (_sprites[state])
		SetCurrentSprite(_sprites[state]);
}
