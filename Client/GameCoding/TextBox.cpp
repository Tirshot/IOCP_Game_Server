#include "pch.h"
#include "TextBox.h"
#include "Sprite.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "InputManager.h"

TextBox::TextBox()
{
	_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");

	_rect.left = _pos.x + _leftPadding;
	_rect.right = _rect.left + _size.x - _leftPadding;
	_rect.top = _pos.y + _topPadding;
	_rect.bottom = _rect.top + _size.y - _topPadding;
}

TextBox::TextBox(wstring wstr)
{
	_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
	_text = wstr;

	_rect.left = _pos.x + _leftPadding;
	_rect.right = _rect.left + _size.x - _leftPadding;
	_rect.top = _pos.y + _topPadding;
	_rect.bottom = _rect.top + _size.y - _topPadding;
}

TextBox::~TextBox()
{
	_backGround = nullptr;
}

void TextBox::BeginPlay()
{
	Super::BeginPlay();
}

void TextBox::Tick()
{
	Super::Tick();

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
			}
			_initialized = true;
		}

		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

		// 아이템 이름이 우측 창을 넘으면 왼쪽으로 표시
		if (mousePos.x + _size.x > 800)
		{
			_rect.left = mousePos.x + _leftPadding - _size.x;
			_rect.right = _rect.left + _size.x - _leftPadding;
			_rect.top = mousePos.y + _topPadding - 26;
			_rect.bottom = _rect.top + _size.y;
		}
		else
		{
			_rect.left = mousePos.x + _leftPadding;
			_rect.right = _rect.left + _size.x - _leftPadding;
			_rect.top = mousePos.y + _topPadding - 26;
			_rect.bottom = _rect.top + _size.y;
		}
	}
	else
	{
		_rect.left = _pos.x + _leftPadding;
		_rect.right = _rect.left + _size.x - _leftPadding;
		_rect.top = _pos.y + _topPadding;
		_rect.bottom = _rect.top + _size.y;
	}
}

void TextBox::Render(HDC hdc)
{
	if (_text == L"")
		return;

	// 텍스트 크기 계산
	RECT calcRect = _rect;
	::DrawTextW(hdc, _text.c_str(), -1, &calcRect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);

	// _rect 크기 조정
	_rect.bottom = calcRect.bottom;

	// 배경 그리기
	{
		if (_transparent == false)
		{
			::StretchBlt(hdc,
				_rect.left - _leftPadding,
				_rect.top - _topPadding,
				_rect.right - _rect.left + _leftPadding,
				_rect.bottom - _rect.top + _topPadding,
				_backGround->GetDC(),
				0,
				0,
				_backGround->GetSize().x,
				_backGround->GetSize().y - 10,
				SRCCOPY);
		}
		else
		{
			BLENDFUNCTION bf = {};
			bf.AlphaFormat = 0; // 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER; // 원본과 대상 이미지를 합침
			bf.SourceConstantAlpha = _alpha; // 투명도(투명 0 - 불투명 255)

			::AlphaBlend(hdc,
				_rect.left - _leftPadding,
				_rect.top - _topPadding,
				_rect.right - _rect.left + _leftPadding,
				_rect.bottom - _rect.top + _topPadding,
				_backGround->GetDC(),
				0,
				0,
				_backGround->GetSize().x,
				_backGround->GetSize().y,
				bf);
		}
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

	Super::Render(hdc);
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
