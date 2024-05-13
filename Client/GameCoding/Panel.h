#pragma once
#include "UI.h"

class Panel : public UI
{
	using Super = UI;

	enum class OverlapDir
	{
		None,
		Left,
		Right,
		Top,
		Bottom,
		Full
	};

public:
	Panel();
	virtual ~Panel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void AddChild(UI* ui);

	template <typename T>
	T* FindChild(const vector<UI*>& children)
	{
		for (auto& item : children)
		{
			T* foundItem = dynamic_cast<T*>(item);
			if (foundItem)
				return foundItem;
		}
		return nullptr;
	}
	virtual RECT GetRect() override;
	bool RemoveChild(UI* ui);
	auto& GetChildren() { return _children; }

	void UpdateChildPos(Panel* parent, int deltaX, int deltaY);
	void DragAndMove(RECT* rect);
	void SetRelativePos(Vec2Int pos);

	void MoveUIToFront(UI* ui);

	bool IsOverlappedWithVisibleUIRect(RECT& thisRect);
	bool IsRectOverlapped(RECT& thisRect, RECT& other);
	bool IsChildPopUpVisible();
	bool IsAnyPopUpVisible();

public:
	void ResetPos();

protected:
	vector<UI*> _children;
	bool _isDragging = false;
	POINT _initialMousePos;
	POINT _mousePos;
	int _offsetX;
	int _offsetY;
};

