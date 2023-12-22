#pragma once
#include "Panel.h"
class Sprite;
class WeaponSlot : public Panel
{
	using Super = Panel;

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
	// UI�� �簢���̶�� ����

	int16 HighlightSlot();

protected:
	Vec2	_pos = { 0, 0 };
	Vec2Int _size = { 52, 52};
	vector<Sprite*> _slots;
	Sprite* _woodenSlot;
	Sprite* _selectedSlot;
};

