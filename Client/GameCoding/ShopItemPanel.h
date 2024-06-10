#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class ShopItemPanel : public Panel
{
	using Super = UI;

public:
	ShopItemPanel();
	ShopItemPanel(ITEM item);
	ShopItemPanel(ITEM item, int index, Vec2 initialPos);
	virtual ~ShopItemPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	RECT GetRect() { return _rect; }
	shared_ptr<ITEM> GetItem() { return _item;  }

	int GetItemIndex() { return _index; }

	void OnPopClickAcceptDelegate();

protected:
	shared_ptr<Texture> _background = nullptr;
	shared_ptr<Texture> _goldImage = nullptr;
	shared_ptr<ITEM> _item = nullptr;
	RECT _rect;
	shared_ptr<class TextBox> _description = nullptr;
	shared_ptr<class TextBox> _itemName = nullptr;
	shared_ptr<class ItemCountsPopUp> _countsPopUp = nullptr;
	int _index = 0;
};

