#pragma once
#include "PopUp.h"

class Button;
class AlertBox :  public PopUp
{
	using Super = PopUp;

public:
	AlertBox();
	virtual ~AlertBox();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

private:
	void OnClickAcceptButton();
	void OnClickDenyButton();

public:
	shared_ptr<Button> MakeAcceptButton();
	shared_ptr<Button> MakeDenyButton();
	void SetIcon(wstring wstr);
	void SetText(wstring wstr);

private:
	shared_ptr<class Sprite> _background = nullptr;
	shared_ptr<class Sprite> _icon = nullptr;
	shared_ptr<class TextBox> _text = nullptr;
	RECT _rect = {};
	Vec2 _initialPos = {};
	bool _result = false;
	int _buttonsCount = 0;
};

