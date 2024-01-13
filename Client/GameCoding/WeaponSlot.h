#pragma once
#include "UI.h"
class Sprite;
class WeaponSlot : public UI
{
	using Super = UI;
#define MAX_SLOT 10
public:
	WeaponSlot();
	virtual ~WeaponSlot();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	Vec2Int GetSize() { return _size; }
	void SetSize(Vec2Int size) { _size = size; }

	Vec2 GetPos() { return _pos; }
	void SetPos(Vec2 pos) { _pos = pos; }
	// UI를 사각형이라고 가정

	int16 HighlightSlot();

	vector<Sprite*> AddSlot(Sprite* sprite) { _slots.insert({ _num++, sprite}); }

protected:
	Vec2	_pos = { 0, 0 };
	Vec2Int _size = { 52, 52};
	map<int, Sprite*> _slots;
	Sprite* _woodenSlot = nullptr;
	Sprite* _selectedSlot = nullptr;
	Sprite* _selected = nullptr;

private:
	Sprite* _weaponSword = nullptr;
	Sprite* _weaponBow = nullptr;
	Sprite* _weaponStaff = nullptr;

	int _num = 1;
};

