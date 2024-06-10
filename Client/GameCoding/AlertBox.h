#pragma once
#include "PopUp.h"
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
	void SetIcon(wstring wstr);
	void SetText(wstring wstr);

private:
	class shared_ptr<Sprite> _background = nullptr;
	class shared_ptr<Sprite> _icon = nullptr;
	RECT _rect = {};
	Vec2 _initialPos = {};
	bool _result = false;
};

