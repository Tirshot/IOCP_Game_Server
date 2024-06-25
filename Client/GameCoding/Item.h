#pragma once
#include "GameObject.h"
class Sprite;
class Item : public GameObject, public enable_shared_from_this<Item>
{
	using Super = GameObject;
public:
	Item();
	virtual ~Item() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetItemInfo(Protocol::ItemInfo info) { itemInfo = info; }
	Protocol::ItemInfo GetItemInfo() { return itemInfo; }

protected:
	virtual void TickIdle();
	shared_ptr<Sprite> _sprite = nullptr;
	wstring _name = L"";
	wstring _korName = L"";

public:
	Protocol::ItemInfo itemInfo;
};

