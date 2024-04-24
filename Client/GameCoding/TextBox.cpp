#include "pch.h"
#include "TextBox.h"
#include "Sprite.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "InputManager.h"

TextBox::TextBox()
{
	_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
}

TextBox::TextBox(wstring wstr)
{
	_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
	_text = wstr;
}

TextBox::~TextBox()
{
}

void TextBox::BeginPlay()
{

}

void TextBox::Tick()
{

}

void TextBox::Render(HDC hdc)
{
	if (_isFloating)
	{
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

		::StretchBlt(hdc,
			mousePos.x,
			mousePos.y,
			_size.x,
			_size.y,
			_backGround->GetDC(),
			0,
			0,
			_backGround->GetSize().x,
			_backGround->GetSize().y,
			SRCCOPY);

		_rect.left = mousePos.x + _leftPadding;
		_rect.right = _rect.left + _size.x;
		_rect.top = mousePos.y + _topPadding;
		_rect.bottom = _rect.top + _size.y;
	}
	else
	{
		::StretchBlt(hdc,
			_pos.x,
			_pos.y,
			_size.x,
			_size.y,
			_backGround->GetDC(),
			0,
			0,
			_backGround->GetSize().x,
			_backGround->GetSize().y,
			SRCCOPY);

		_rect.left = _pos.x + _leftPadding;
		_rect.right = _rect.left + _size.x;
		_rect.top = _pos.y + _topPadding;
		_rect.bottom = _rect.top + _size.y;
	}


	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	switch (_textAlign)
	{
	case TextAlign::Left:
		::DrawTextW(hdc, _text.c_str(), -1, &_rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK);
		break;

	case TextAlign::Center:
		::DrawTextW(hdc, _text.c_str(), -1, &_rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		break;

	case TextAlign::Right:
		::DrawTextW(hdc, _text.c_str(), -1, &_rect, DT_RIGHT | DT_VCENTER | DT_WORDBREAK);
		break;
	}
}

void TextBox::SetText(wstring text)
{
	_text = text;
}

void TextBox::SetPadding(float x, float y)
{
	_leftPadding = x;
	_topPadding = y;
}
