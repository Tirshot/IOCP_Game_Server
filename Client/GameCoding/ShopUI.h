#pragma once
#include "Panel.h"
class Texture;
class ShopUI :  public Panel
{
	using Super = Panel;

public:
	ShopUI();
	virtual ~ShopUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void OnClickBackButton();
	void OnClickExitButton();

	void SetAllCost(int cost) { _allCost = cost; }
	int GetAllCost();

protected:
	RECT _rect = {};
	Texture* _background = nullptr;
	map<int, int> _costs = {};
	int _allCost = 0;
};
extern int g_itemidGenarator;
