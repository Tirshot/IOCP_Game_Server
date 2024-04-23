#include "pch.h"
#include "Panel.h"
#include "InputManager.h"

Panel::Panel()
{

}

Panel::~Panel()
{
	for (UI* child : _children)
		SAFE_DELETE(child);

	_children.clear();
}

void Panel::BeginPlay()
{
	Super::BeginPlay();

	for (UI* child : _children)
		child->BeginPlay();
}

void Panel::Tick()
{
	Super::Tick();

	for (UI* child : _children)
		child->Tick();
}

void Panel::Render(HDC hdc)
{
	Super::Render(hdc);

	for (UI* child : _children)
		child->Render(hdc);
}

void Panel::AddChild(UI* ui)
{
	if (ui == nullptr)
		return;

	ui->SetParent(this);

	_children.push_back(ui);
}

bool Panel::RemoveChild(UI* ui)
{
	auto findIt = std::find(_children.begin(), _children.end(), ui);
	// 해당하는 ui를 찾지 못했다면
	if (findIt == _children.end())
		return false;

	_children.erase(findIt);
	return true;
}

void Panel::UpdateChildPos(Panel* parent, int deltaX, int deltaY)
{
	// 자식 위치 업데이트
	if (parent)
		for (auto& child : parent->GetChildren())
		{
			if (child->GetParent() != parent)
				continue;

			Vec2 childPos = child->GetPos();
			child->SetPos({ childPos.x + deltaX, childPos.y + deltaY });

			UpdateChildPos(dynamic_cast<Panel*>(child), deltaX, deltaY);
		}
}

void Panel::DragAndMove(RECT* rect)
{
	_mousePos = GET_SINGLE(InputManager)->GetMousePos();
	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;

	// 인벤토리 창 드래그
	if (IsMouseInRect(*rect))
	{
		// 드래그 시작
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialPos = _mousePos;
			//
			_offsetX = (int)_mousePos.x - (int)rect->left;
			_offsetY = (int)_mousePos.y - (int)rect->top;
			_isDragging = true;
		}
	}

	// 드래그 중 위치 업데이트
	if (_isDragging)
	{
		// 클릭한 위치 기준 이동
		int newLeft = _mousePos.x - _offsetX;
		int newTop = _mousePos.y - _offsetY;

		// 이동량 계산 (드래그로 인한 차이)
		int deltaX = newLeft - rect->left;
		int deltaY = newTop - rect->top;

		// rect와 _pos 업데이트
		rect->left = newLeft;
		rect->top = newTop;
		rect->right = newLeft + width;
		rect->bottom = newTop + height;
		_pos.x = newLeft;
		_pos.y = newTop;

		// 자식 위치 업데이트
		UpdateChildPos(this, deltaX, deltaY);
		//for (auto& child : _children)
		//{
		//	Vec2 childPos = child->GetPos();
		//	child->SetPos({ childPos.x + deltaX, childPos.y + deltaY });
		//}
	}

	// 드래그 종료
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
	{
		_isDragging = false;
	}
}
