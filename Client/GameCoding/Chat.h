#pragma once
#include "Panel.h"

class Sprite;
class Chat :  public Panel
{
public:
	Chat();
	Chat(Vec2 rectPos);
	virtual ~Chat();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetRectPos(int posX, int posY);

	// 텍스트 관련
	void AddText(const wstring str);
	vector<wstring>& GetTexts() { return _texts; }

private:
	RECT _rect = {};
	Vec2 _rectPos = { 0,0};
	Sprite* _chatSprite = nullptr;
	float _sumTime = 0;
	vector<wstring> _texts;
};

