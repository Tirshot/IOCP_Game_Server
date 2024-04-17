#pragma once
#include "UI.h"
class Sprite;
class TextBox : public UI
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
	void SetText(wstring text);

	void SetPadding(float x, float y);

protected:
	Sprite* _backGround = nullptr;
	RECT _rect = {};
	wstring _text = L"";

private:
	float _leftPadding = 15.f;
	float _topPadding = 15.f;
};

