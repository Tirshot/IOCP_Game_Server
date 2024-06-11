#pragma once
#include "UI.h"
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
	void RemoveTextChar(WCHAR text[], int len);
	void ClearTextBox();

	RECT GetRect() { return _rect; }

protected:
	RECT _rect = {};
	HDC _hdc = nullptr;

	WCHAR _text[256] = {};
};

