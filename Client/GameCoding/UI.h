#pragma once
class UI
{ // UI���� Scene���� ��� �ִ�
public:
	UI();
	virtual ~UI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetPos(Vec2 pos) { _pos = pos; }
	Vec2 GetPos(){ return _pos; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

	void SetVisible(bool visible) { if (_visible == visible) return; _visible = visible; }
	bool GetVisible() const { return _visible; }

	// UI�� �簢���̶�� ����
	RECT GetRect();
	bool IsMouseInRect();

protected:
	Vec2	_pos = { 0, 0 };
	Vec2Int _size = { 150, 150 };
	bool _visible = true;
};

