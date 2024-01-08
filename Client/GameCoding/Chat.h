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
	void ChatBoxFade();

	int SetAlpha(int alpha) { return _alpha = alpha; }
	float SetSumTime(float time) { return _sumTime = time; }

	bool GetChatState() { return _chatState; }
	void SetChatState(bool state) { _chatState = state; }

	int GetLineHeight(int idx);

private:
	Sprite* _chatSprite = nullptr;
	RECT _rect = {};
	Vec2 _rectPos = { 0,0};

	vector<wstring> _texts;
	int _alpha = 0;
	float _sumTime = 0;
	bool _chatState = false;

public:
	vector<int> _height;
};

