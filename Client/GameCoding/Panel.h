#pragma once
#include "UI.h"

class Panel : public UI, public enable_shared_from_this<Panel>
{
	using Super = UI;

public:
	Panel();
	virtual ~Panel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void AddChild(shared_ptr<UI> ui);

	template <typename T>
	shared_ptr<T> FindChild(const vector<shared_ptr<UI>>& children)
	{
		for (auto& item : children)
		{
			shared_ptr<T> foundItem = dynamic_pointer_cast<T>(item);
			if (foundItem)
				return foundItem;
		}
		return nullptr;
	}
	virtual RECT GetRect() override;
	bool RemoveChild(shared_ptr<UI> ui);
	auto& GetChildren() { return _children; }

	void UpdateChildPos(shared_ptr<Panel> parent, int deltaX, int deltaY);
	void DragAndMove(RECT rect);
	void SetRelativePos(Vec2Int pos);

	void MoveUIToFront(shared_ptr<UI> ui);

	bool IsOverlappedWithVisibleUIRect(RECT& thisRect);
	bool IsRectOverlapped(RECT& thisRect, RECT& other);
	bool IsChildPopUpVisible();
	bool IsAnyPopUpVisible();

public:
	void ResetPos();

protected:
	vector<shared_ptr<UI>> _children;
	bool _isDragging = false;
	POINT _initialMousePos;
	POINT _mousePos;
	int _offsetX;
	int _offsetY;
};

