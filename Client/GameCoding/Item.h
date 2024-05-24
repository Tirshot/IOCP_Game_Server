#pragma once
#include "GameObject.h"
class Sprite;
class Item : public GameObject
{
	using Super = GameObject;
public:
	Item();
	virtual ~Item() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetItemInfo(Protocol::ItemInfo info) { itemInfo = info; }

protected:
	virtual void TickIdle();
	Sprite* _sprite = nullptr;
	wstring _name = L"";
	wstring _korName = L"";

private:
	Protocol::ItemInfo itemInfo;
};

