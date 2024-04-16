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
	void ChangeText(wstring text);

protected:
	Sprite* _backGround = nullptr;
	RECT _rect = {};
	wstring _text = L"";
};

