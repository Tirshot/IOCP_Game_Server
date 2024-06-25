#pragma once
class UI
{ // UI들은 Scene에서 들고 있다
public:
	UI();
	virtual ~UI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetPos(Vec2 pos) { _pos = pos;/*	_initialPos = _pos;*/}
	Vec2 GetPos(){ return _pos; }
	void SetInitialPos(Vec2 pos) { _initialPos = pos; }
	Vec2 GetInitialPos() { return _initialPos; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

	void SetVisible(bool visible) { _visible = visible; }
	bool GetVisible() const { return _visible; }

	void SetParent(shared_ptr<UI> ui) { _parent = ui; }
	shared_ptr<UI> GetParent() { return _parent; }

	void SetPause(bool pause) { _pause = pause; }

	// UI를 사각형이라고 가정
	virtual RECT GetRect();
	bool IsMouseInRect();
	bool IsMouseInRect(RECT rect);
	bool IsMouseOutRect(RECT rect);

	int GetUIID() { return _id; }

protected:
	Vec2	_pos = { 0, 0 };
	Vec2 _initialPos;
	RECT _rect = {};
	Vec2Int _size = { 150, 150 };
	bool _visible = true;
	shared_ptr<UI> _parent = nullptr;
	int _id = 0;
	bool _pause = false;
	float _sumTime = 0.f;

	// id 부여
	static int ui_idGenerator;
};

