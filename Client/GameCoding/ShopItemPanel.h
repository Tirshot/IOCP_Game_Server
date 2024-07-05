#pragma once
#include "Panel.h"
class Texture;
class Sprite;
class ShopItemPanel : public Panel
{
	using Super = Panel;

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
	RECT _rect = {};
	int _index = 0;
};

