#pragma once
#include "Panel.h"
class Sprite;
class TextBox : public Panel
{
	using Super = UI;

public:
	TextBox();
	TextBox(wstring wstr);
	virtual ~TextBox();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

public:
	RECT GetRect() { return _rect; }
	void SetText(wstring text);
	wstring GetText() { return _text; }
	void AlignText(TextAlign align) { _textAlign = align; }
	void SetFloating(bool floating) { _isFloating = floating; }
	void SetPadding(float x, float y);
	bool IsOverlapped(RECT other);
	void SetBackground(Sprite* sprite) { _backGround = sprite; }
	void SetTransparent(bool transparent) { _transparent = transparent; }
	void SetAlpha(int alpha) { _alpha = alpha; }
	void SetRect(int left, int top, int right, int bottom) { _rect.left = left; _rect.top = top; _rect.right = right; _rect.bottom = bottom; }

protected:
	Sprite* _backGround = nullptr;
	RECT _rect = {};
	wstring _text = L"";
	int _alpha = 255;
	bool _transparent = false;

private:
	float _leftPadding = 15.f;
	float _topPadding = 15.f;
	bool _isFloating = false;
	TextAlign _textAlign = TextAlign::Left;
	bool _initialized = false;
	bool _isOverlapped = false;
};

