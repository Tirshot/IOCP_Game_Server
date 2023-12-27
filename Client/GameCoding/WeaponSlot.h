#pragma once
#include "Panel.h"
class Sprite;
class WeaponSlot : public Panel
{
	using Super = Panel;
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

	vector<Sprite*> AddSlot(Sprite* sprite) { _slots.insert({ (int)_slots.size() + 1, sprite}); }

protected:
	Vec2	_pos = { 0, 0 };
	Vec2Int _size = { 52, 52};
	map<int, Sprite*> _slots;
	Sprite* _woodenSlot = nullptr;
	Sprite* _selectedSlot = nullptr;
	Sprite* _selected = nullptr;
};

