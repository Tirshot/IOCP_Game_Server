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
	void AlignText(TextAlign align) { _textAlign = align; }
	void SetFloating(bool floating) { _isFloating = floating; }
	void SetPadding(float x, float y);
	bool IsOverlapped(RECT other);

protected:
	Sprite* _backGround = nullptr;
	RECT _rect = {};
	wstring _text = L"";

private:
	float _leftPadding = 15.f;
	float _topPadding = 15.f;
	bool _isFloating = false;
	TextAlign _textAlign = TextAlign::Left;
	bool _initialized = false;
	bool _isOverlapped = false;
};

