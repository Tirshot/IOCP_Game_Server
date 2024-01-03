#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	HDC GetDC() { return _hdc; }

private:
	HWND _hwnd = {};
	HDC _hdc = {};

private:
	RECT _rect = {};
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};

};
