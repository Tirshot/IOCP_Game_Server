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
	void OnPopClickAcceptDelegate();
	void OnClickBackButton();
	void OnClickExitButton();

private:
	ITEM* GetSellItem(int itemID);
	int GetItemIndex(int itemID);
	void SetAllCost(int cost) { _allCost = cost; }
	void SetChildVisible(bool visible);


protected:
	RECT _rect = {};
	RECT _dragRect = {};
	Texture* _background = nullptr;
	class ItemCountsPopUp* _countsPopUp = nullptr;
	vector<ITEM*> _items = {};	// 
	ITEM* _sellItem = {};
	int _counts = 1;
	int _price = 0;
	int _allCost = 0;
};
