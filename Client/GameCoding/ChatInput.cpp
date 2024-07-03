#include "pch.h"
#include "ChatInput.h"
#include "Sprite.h"
#include "ChatManager.h"

ChatInput::ChatInput()
{
	_visible = false;
	_pos = { 10, 530 };
	_size = { 300, 20 };
}

ChatInput::~ChatInput()
{

}

void ChatInput::BeginPlay()
{

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
		{
			// Input text
			SetBkMode(hdc, BLACKNESS);
			SetTextColor(hdc, RGB(230, 230, 230));
			RECT textRect = { _pos.x + 5, _pos.y - 45, textRect.left + _size.x, textRect.top + 35 };
			DrawTextW(hdc, _text, -1,&textRect, DT_LEFT);
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

void ChatInput::RemoveTextChar(WCHAR text[], int len)
{
	_text[len] = L' ';
}

void ChatInput::ClearTextBox()
{
	// _text ÃÊ±âÈ­
	memset(_text, 0, 255);
}