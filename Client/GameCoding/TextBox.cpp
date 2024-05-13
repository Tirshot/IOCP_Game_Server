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
		if (_text == L"")
			return;

		// 1회만 실행
		if (_initialized == false)
		{
			_size.x = _text.size() * 19;

			if (_text.size() == 1)
				_size.x = 25;

			for (wchar_t ch : _text)
			{
				if (ch == L' ') // 스페이스바(공백)을 감지
				{
					_size.x -= 19;
				}
				_initialized = true;
			}
		}

		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

		// 아이템 이름이 우측 창을 넘으면 왼쪽으로 표시
		if (mousePos.x + _size.x > 800)
		{
			::StretchBlt(hdc,
				mousePos.x - _size.x,
				mousePos.y - 26,
				_size.x,
				_size.y,
				_backGround->GetDC(),
				0,
				0,
				_backGround->GetSize().x,
				_backGround->GetSize().y,
				SRCCOPY);

			_rect.left = mousePos.x + _leftPadding - _size.x;
			_rect.right = _rect.left + _size.x - _leftPadding;
			_rect.top = mousePos.y + _topPadding - 26;
			_rect.bottom = _rect.top + _size.y - _topPadding;
		}
		else
		{
			::StretchBlt(hdc,
				mousePos.x,
				mousePos.y - 26,
				_size.x,
				_size.y,
				_backGround->GetDC(),
				0,
				0,
				_backGround->GetSize().x,
				_backGround->GetSize().y,
				SRCCOPY);

			_rect.left = mousePos.x + _leftPadding;
			_rect.right = _rect.left + _size.x - _leftPadding;
			_rect.top = mousePos.y + _topPadding - 26;
			_rect.bottom = _rect.top + _size.y - _topPadding;
		}
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
		_rect.right = _rect.left + _size.x - _leftPadding;
		_rect.top = _pos.y + _topPadding;
		_rect.bottom = _rect.top + _size.y - _topPadding;
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
	_initialized = false;
}

void TextBox::SetPadding(float x, float y)
{
	_leftPadding = x;
	_topPadding = y;
}

bool TextBox::IsOverlapped(RECT other)
{
	RECT thisRect = _rect;

	return !(thisRect.right < other.left || thisRect.left > other.right ||
		thisRect.bottom < other.top || thisRect.top > other.bottom);

	return false;
}
