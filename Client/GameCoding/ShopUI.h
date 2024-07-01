#pragma once
#include "Panel.h"
class Texture;
class AlertBox;
class ItemCountsPopUp;
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
	void ResetPage() { _page = 1; SetPause(false); }

	RECT GetShopRect() { return _rect; }

public:
	shared_ptr<AlertBox> MakeAlertBox(Vec2 pos, Vec2Int size, void (ShopUI::* func)() = nullptr, bool twoButtons = true);
	shared_ptr<ItemCountsPopUp> MakeCountsBox(Vec2 pos, Vec2Int size, int itemID, void (ShopUI::* func)() = nullptr);

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
	vector<shared_ptr<ITEM>> _items = {};	// �Ǹ��� �������� ����Ǿ� ����
	shared_ptr<ITEM> _sellItem = {};
	shared_ptr<ITEM> _sellToShop = {};
	int _counts = 1;	// ������ ���� ����
	int _price = 0;
	int _allCost = 0;
	float _initializeTime = 0.f;	// �Ǹ�â ǰ�� ���� ���ð�
	int _page = 1;	// ���� ������

private:
	int _buyPriceDivider = 5; // ������ �������� �Ǹ��� �� �޴� ��
};
