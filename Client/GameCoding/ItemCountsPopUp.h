#pragma once
#include "PopUp.h"
class ItemCountsPopUp : public PopUp
{
	using Super = PopUp;

public:
	ItemCountsPopUp();
	virtual ~ItemCountsPopUp();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void SetPos(Vec2 pos) { _pos = { pos.x - (int)_size.x / 2, pos.y - (int)_size.y / 2 }; }

private:
	void OnClickAcceptButton();
	void OnClickDenyButton();
	void OnClickCountPlusButton();
	void OnClickCountDoublePlusButton();
	void OnClickCountMinusButton();
	void OnClickCountDoubleMinusButton();


public:
	void SetIcon(wstring wstr);
	void SetText(wstring wstr);
	RECT GetRect() { return _rect; }
	int GetCounts() { return _counts; }
	void SetPrice(int price) { _price = price; }
	int GetPrice() { return _price; }

private:
	class Sprite* _background = nullptr;
	class Sprite* _icon = nullptr;
	RECT _rect = {};
	int _counts = 1;
	int _price = 0;
	int _totalPrice = 0;
};

