#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class ShopItemPanel : public Panel
{
	using Super = UI;

public:
	ShopItemPanel();
	ShopItemPanel(Sprite* texture, int cost, wstring name, wstring text, Vec2 pos);
	virtual ~ShopItemPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void SetItemCost(int cost) { _itemCost = cost; }
	void ResetItemCount() { _itemCount = 0; }

	void OnClickLLButton();
	void OnClickLButton();
	void OnClickRRButton();
	void OnClickRButton();
	void OnClickPurchaseButton();

	int GetCost() { return _allCost; }
	int GetItemID() { return _itemID; }

protected:
	Texture* _background = nullptr;
	Sprite* _itemImage = nullptr;
	Texture* _goldImage = nullptr;
	int _itemID = 0; /* 0 : arrow, 1 : maxHP, 2 : potion*/
	wstring _itemText = L"";
	wstring _itemName = L"";
	int _itemCount = 0;
	int _itemCost = 0;
	// 한 칸내 최종 가격
	int _allCost = 0;
};

