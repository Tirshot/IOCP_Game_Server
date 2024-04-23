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

public:
	void AddSellItem(ITEM* item);
	void AddSellItem(int itemID);

private:
	void OnClickBackButton();
	void OnClickExitButton();

private:
	ITEM* GetSellItem(int itemID);
	int GetItemIndex(int itemID);
	void SetAllCost(int cost) { _allCost = cost; }


protected:
	RECT _rect = {};
	Texture* _background = nullptr;
	vector<ITEM*> _items = {};	// 
	int _allCost = 0;
};
