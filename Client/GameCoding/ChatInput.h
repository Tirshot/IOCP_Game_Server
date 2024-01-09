#pragma once
#include "UI.h"
class Sprite;
class ChatInput : public UI
{
public:
	ChatInput();
	virtual ~ChatInput();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetRectPos(int posX, int posY);
	void SetPos(Vec2 pos) { _pos = Vec2{ pos.x, pos.y }; }
	void SetSize(Vec2Int size) { _size = { size.x, size.y }; }

	void AddTextChar(WCHAR text[], int len);
	void ClearTextBox();

	HDC GetDC();
	RECT GetRect() { return _rect; }

protected:
	Vec2	_pos = { 10, 520 };
	Sprite* _sprite = nullptr;
	RECT _rect = {};
	HDC _hdc = nullptr;

	Vec2Int _size = { 300, 20 };

	WCHAR _text[256];
};

