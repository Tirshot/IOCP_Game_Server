#pragma once
#include "UI.h"
class Sprite;
class NamePlate : public UI
{
	using Super = UI;
public:
	NamePlate();
	NamePlate(wstring wstr);
	virtual ~NamePlate();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	wstring GetText() { return _name; }

protected:
	Sprite* _sprite = nullptr;
	RECT _rect = {};
	wstring _name = L"";
};

