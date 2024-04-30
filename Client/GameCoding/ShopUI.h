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

	void ResetInitializingTime() { _initializeTime = 0.f; }

private:
	void OnPopClickAcceptDelegate();
	void OnClickBackButton();
	void OnClickExitButton();

private:
	ITEM* GetSellItem(int itemID);
	int GetItemIndex(int itemID);
	void SetAllCost(int cost) { _allCost = cost; }
	void SetChildVisible(bool visible);
	
	void OnClickCountMinusButton();
	void OnClickCountPlusButton();


protected:
	RECT _rect = {};
	RECT _dragRect = {};
	Texture* _background = nullptr;
	class ItemCountsPopUp* _countsPopUp = nullptr;
	class	AlertBox* _alert = {};
	vector<ITEM*> _items = {};	// �Ǹ��� �������� ����Ǿ� ����
	ITEM* _sellItem = {};
	int _counts = 1;	// ������ ���� ����
	int _price = 0;
	int _allCost = 0;
	float _initializeTime = 0.f;	// �Ǹ�â ǰ�� ���� ���ð�
	int _page = 1;	// ���� ������
};
