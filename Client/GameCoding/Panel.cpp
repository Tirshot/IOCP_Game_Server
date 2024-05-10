#include "pch.h"
#include "Panel.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Button.h"
#include "AlertBox.h"
#include "DevScene.h"
#include "ItemCountsPopUp.h"

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

	_initialPos = _pos;

	for (UI* child : _children)
		child->BeginPlay();
}

void Panel::Tick()
{
	Super::Tick();

	if (_visible == false)
		_pos = _initialPos;

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

			// AlertBox가 따라 움직이는 버그 수정
			if (dynamic_cast<AlertBox*>(child))
				continue;

			// ItemCountsPopUp이 따라 움직이는 버그 수정
			if (dynamic_cast<ItemCountsPopUp*>(child))
				continue;

			Vec2 childPos = child->GetPos();
			child->SetPos({ childPos.x + deltaX, childPos.y + deltaY });

			UpdateChildPos(dynamic_cast<Panel*>(child), deltaX, deltaY);
		}
}

void Panel::DragAndMove(RECT* rect)
{
	_mousePos = GET_SINGLE(InputManager)->GetMousePos();
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;

	// 여러 개의 패널이 띄워져 있을 때 가장 나중에 생성된 패널만 드래그 가능
	if (_isDragging)
	{	
		// 두 개의 드래그 가능한 창이 존재할 때 자식의 창만 드래그 가능
		for (auto& child : _children)
		{
			Panel* panel = dynamic_cast<Panel*>(child);
			if (panel)
			{
				if (panel->_isDragging)
				{
					this->_isDragging = false;
					return;
				}
			}
		}
		if (scene)
		{
			auto visibleUIs = scene->GetVisibleUIs();

			if (visibleUIs.empty() == false)
			{
				for (auto& ui : visibleUIs)
				{
					Panel* panel = dynamic_cast<Panel*>(ui);
					if (panel)
					{
						if (panel->_isDragging && panel->GetUIID() > this->GetUIID())
						{
							this->_isDragging = false;
							return;
						}
					}
				}
			}
		}
	}

	// 인벤토리 창 드래그
	if (IsMouseInRect(*rect))
	{
		// 드래그 시작
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialMousePos = _mousePos;
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

void Panel::SetRelativePos(Vec2Int pos)
{
	// 부모의 패널 중앙 기준
	if (_parent == nullptr)
		return;

	Vec2 Pos = _parent->GetPos();
	Vec2Int Size = _parent->GetSize();

	SetPos({ (int)Pos.x + ((float)Size.x / 2) + (int)pos.x, (int)Pos.y + ((float)Size.y / 2) + (int)pos.y});
}

bool Panel::IsOverlapped(RECT& thisRect, RECT& other)
{
	// 두 사각형이 겹치는지 여부를 판단
	return !(thisRect.right < other.left || thisRect.left > other.right ||
		thisRect.bottom < other.top || thisRect.top > other.bottom);
}

void Panel::ResetPos()
{
	// 기본 위치로 리셋
	_pos = _initialPos;

	for (auto& child : _children)
	{
		auto pos = child->GetInitialPos();
		child->SetPos(pos);

		auto panel = dynamic_cast<Panel*>(child);

		// panel이면서 자식이 있는 경우 자식에 대해 ResetPos 실행
		if (panel && panel->GetChildren().size() > 0)
			panel->ResetPos();
	}
}
