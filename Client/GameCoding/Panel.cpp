#include "pch.h"
#include "Panel.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Button.h"
#include "AlertBox.h"
#include "Inventory.h"
#include "DevScene.h"
#include "ItemCountsPopUp.h"

Panel::Panel()
{

}

Panel::~Panel()
{
	_children.clear();
}

void Panel::BeginPlay()
{
	_initialPos = _pos;

	for (auto& child : _children)
		child->BeginPlay();
}

void Panel::Tick()
{
	if (_visible == false)
		_pos = _initialPos;

	for (auto& child : _children)
		child->Tick();
}

void Panel::Render(HDC hdc)
{
	for (auto& child : _children)
		child->Render(hdc);
}

void Panel::AddChild(shared_ptr<UI> ui)
{
	if (ui == nullptr)
		return;

	ui->SetParent(shared_from_this());

	_children.push_back(ui);
}

RECT Panel::GetRect()
{
	return _rect;
}

bool Panel::RemoveChild(shared_ptr<UI> ui)
{
	auto findIt = std::find(_children.begin(), _children.end(), ui);
	// 해당하는 ui를 찾지 못했다면
	if (findIt == _children.end())
		return false;

	_children.erase(findIt);
	return true;
}

void Panel::UpdateChildPos(shared_ptr<Panel> parent, int deltaX, int deltaY)
{
	// 자식 위치 업데이트
	if (parent)
		for (auto& child : parent->GetChildren())
		{
			if (child->GetParent() != parent)
				continue;

			// AlertBox가 따라 움직이는 버그 수정
			if (dynamic_pointer_cast<AlertBox>(child))
				continue;

			// ItemCountsPopUp이 따라 움직이는 버그 수정
			if (dynamic_pointer_cast<ItemCountsPopUp>(child))
				continue;

			Vec2 childPos = child->GetPos();
			child->SetPos({ childPos.x + deltaX, childPos.y + deltaY });

			UpdateChildPos(dynamic_pointer_cast<Panel>(child), deltaX, deltaY);
		}
}

void Panel::DragAndMove(RECT rect)
{
	_mousePos = GET_SINGLE(InputManager)->GetMousePos();
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 여러 개의 패널이 띄워져 있을 때 가장 나중에 생성된 패널만 드래그 가능
	if (_isDragging)
	{	
		// 두 개의 드래그 가능한 창이 존재할 때 자식의 창만 드래그 가능
		for (auto& child : _children)
		{
			shared_ptr<Panel> panel = dynamic_pointer_cast<Panel>(child);
			if (panel && panel->GetVisible() == true && panel->_isDragging)
			{
				this->_isDragging = false;
				return;
			}
		}

		if (IsAnyPopUpVisible())
		{
			this->_isDragging = false;
			return;
		}
	}

	// 인벤토리 창 드래그
	if (IsMouseInRect(rect))
	{
		// 드래그 시작
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialMousePos = _mousePos;
			//
			_offsetX = (int)_mousePos.x - (int)rect.left;
			_offsetY = (int)_mousePos.y - (int)rect.top;
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
		int deltaX = newLeft - rect.left;
		int deltaY = newTop - rect.top;

		// rect와 _pos 업데이트
		rect.left = newLeft;
		rect.top = newTop;
		rect.right = newLeft + width;
		rect.bottom = newTop + height;
		_pos.x = newLeft;
		_pos.y = newTop;

		auto parent = static_pointer_cast<Panel>(shared_from_this());

		if (parent)
		{
			// 자식 위치 업데이트
			UpdateChildPos(parent, deltaX, deltaY);
		}

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

void Panel::MoveUIToFront(shared_ptr<UI> ui)
{
	int index = 0;

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return;

	auto& vec = scene->GetUIRef();

	for (auto& v : vec)
	{
		index++;

		if (v == ui)
			break;
	}

	if (index < vec.size())
	{
		// 중간 요소를 끝으로 이동
		::rotate(vec.begin() + index, vec.begin() + index + 1, vec.end());
	}
}

bool Panel::IsOverlappedWithVisibleUIRect(RECT& thisRect)
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		auto visibleUIs = scene->GetVisibleUIs();
		int visibleUICount = visibleUIs.size();

		if (visibleUIs.empty() == false)
		{
			for (int i = 0; i < visibleUICount; i++)
			{
				auto panel = dynamic_pointer_cast<Panel>(visibleUIs[i]);
				if (panel)
				{
					if (shared_from_this() == panel)
						continue;

					const auto& other = panel->GetRect();

					// 두 사각형이 겹치는지 여부를 판단
					return !(thisRect.right < other.left || thisRect.left > other.right ||
						thisRect.bottom < other.top || thisRect.top > other.bottom);
				}
			}
		}
	}

	return true;
}

bool Panel::IsRectOverlapped(RECT& thisRect, RECT& other)
{
	// 두 사각형이 겹치는지 여부를 판단
	return !(thisRect.right < other.left || thisRect.left > other.right ||
		thisRect.bottom < other.top || thisRect.top > other.bottom);
}

bool Panel::IsChildPopUpVisible()
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		if (_children.empty() == false)
		{
			for (auto& panel : _children)
			{
				auto popUp = dynamic_pointer_cast<PopUp>(panel);
				if (popUp && popUp->GetVisible())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Panel::IsAnyPopUpVisible()
{
	if (_visible)
	{
		auto scene = GET_SINGLE(SceneManager)->GetDevScene();
		if (scene)
		{
			auto visibleUIs = scene->GetVisibleUIs();
			if (visibleUIs.empty() == false)
			{
				for (auto& ui : visibleUIs)
				{
					auto panel = dynamic_pointer_cast<Panel>(ui);
					if (panel == nullptr)
						continue;

					auto& children = panel->GetChildren();

					if (children.empty())
						continue;

					for (auto& child : children)
					{
						auto popUp = dynamic_pointer_cast<PopUp>(child);

						if (popUp == nullptr)
							continue;

						if (popUp == shared_from_this())
							continue;

						if (popUp->GetVisible())
							return true;
					}
				}
			}
		}
	}
	return false;
}

void Panel::ResetPos()
{
	// 기본 위치로 리셋
	_pos = _initialPos;

	for (auto& child : _children)
	{
		auto pos = child->GetInitialPos();
		child->SetPos(pos);

		auto panel = dynamic_pointer_cast<Panel>(child);

		// panel이면서 자식이 있는 경우 자식에 대해 ResetPos 실행
		if (panel && panel->GetChildren().size() > 0)
			panel->ResetPos();
	}
}
