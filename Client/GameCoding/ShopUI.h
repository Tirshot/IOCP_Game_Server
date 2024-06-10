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
	void AddSellItem(shared_ptr<ITEM> item);
	void AddSellItem(int itemID);

	void SellItemToShop(shared_ptr<ITEM> item);

	void ResetInitializingTime() { _initializeTime = 0.f; }
	void ResetPage() { _page = 1; }

	RECT GetShopRect() { return _rect; }

private:
	void OnPopClickAcceptDelegate();
	void OnPopClickAlertAcceptDelegate();
	void OnClickSellItemToShopAlertAcceptDelegate();
	void OnClickBackButton();
	void OnClickExitButton();

private:
	shared_ptr<ITEM> GetSellItem(int itemID);
	int GetItemIndex(int itemID);
	void SetAllCost(int cost) { _allCost = cost; }
	void SetChildVisible(bool visible);
	
	void OnClickCountMinusButton();
	void OnClickCountPlusButton();


protected:
	RECT _rect = {};
	RECT _dragRect = {};
	shared_ptr<Texture> _background = nullptr;
	shared_ptr<class ItemCountsPopUp> _countsPopUp = nullptr;
	shared_ptr<class	AlertBox> _alert = {};
	vector<shared_ptr<ITEM>> _items = {};	// 판매할 아이템이 저장되어 있음
	shared_ptr<ITEM> _sellItem = {};
	shared_ptr<ITEM> _sellToShop = {};
	int _counts = 1;	// 아이템 구매 갯수
	int _price = 0;
	int _allCost = 0;
	float _initializeTime = 0.f;	// 판매창 품목 구입 대기시간
	int _page = 1;	// 열린 페이지

private:
	int _buyPriceDivider = 5;
};
