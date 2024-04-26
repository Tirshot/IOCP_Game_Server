#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class ShopItemPanel : public Panel
{
	using Super = UI;

public:
	ShopItemPanel();
	ShopItemPanel(ITEM* item);
	ShopItemPanel(ITEM* item, int index, Vec2 initialPos);
	virtual ~ShopItemPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	RECT GetRect() { return _rect; }
	ITEM* GetItem() { return _item;  }

	void OnPopClickAcceptDelegate();

protected:
	Texture* _background = nullptr;
	Texture* _goldImage = nullptr;
	ITEM* _item = nullptr;
	RECT _rect;
	class TextBox* _description = nullptr;
	class TextBox* _itemName = nullptr;
	class ItemCountsPopUp* _countsPopUp = nullptr;
};

