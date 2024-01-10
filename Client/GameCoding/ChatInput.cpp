#include "pch.h"
#include "ChatInput.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "ChatManager.h"

ChatInput::ChatInput()
{
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 300;
	_rect.top = -13;
	_rect.bottom = 20;

	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"ChatInput");

	_visible = false;
}

ChatInput::~ChatInput()
{
}

void ChatInput::BeginPlay()
{
	//_size.x = _rect.right - _rect.left;
	//_size.y = _rect.bottom - _rect.top;
}

void ChatInput::Tick()
{

}

void ChatInput::Render(HDC hdc)
{
	if(_hdc == nullptr)
		_hdc = hdc;

	if (_visible)
	{
		{// 백그라운드
			::BitBlt(hdc,
				(int)_pos.x + 15,
				(int)_pos.y,
				_size.x,
				_size.y,
				_sprite->GetDC(),
				0,
				0,
				SRCCOPY);

			// Input text
			SetBkMode(_sprite->GetDC(), TRANSPARENT);
			SetTextColor(_sprite->GetDC(), RGB(230, 230, 230));
			::DrawTextW(_sprite->GetDC(), _text, -1,&_rect, DT_LEFT | DT_TOP);
		}
	}
}

void ChatInput::SetRectPos(int posX, int posY)
{	// text input box
	_rect.left = posX;
	_rect.right += posX;
	_rect.top = posY;
	_rect.bottom += posY;
}

void ChatInput::AddTextChar(WCHAR text[], int len)
{
	for (int i = 0; i < len; i++)
		_text[i] = text[i];
}

void ChatInput::ClearTextBox()
{
	// 백그라운드를 검은 색으로 지우기
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(_sprite->GetDC(), &_rect, brush);
	DeleteObject(brush);

	// _text 초기화
	memset(_text, 0, 255);
}

HDC ChatInput::GetDC()
{
	return _sprite ? _sprite->GetDC() : 0;
}