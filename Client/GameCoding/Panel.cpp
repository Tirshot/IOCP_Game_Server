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
	// �ش��ϴ� ui�� ã�� ���ߴٸ�
	if (findIt == _children.end())
		return false;

	_children.erase(findIt);
	return true;
}

void Panel::UpdateChildPos(shared_ptr<Panel> parent, int deltaX, int deltaY)
{
	// �ڽ� ��ġ ������Ʈ
	if (parent)
		for (auto& child : parent->GetChildren())
		{
			if (child->GetParent() != parent)
				continue;

			// AlertBox�� ���� �����̴� ���� ����
			if (dynamic_pointer_cast<AlertBox>(child))
				continue;

			// ItemCountsPopUp�� ���� �����̴� ���� ����
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

	// ���� ���� �г��� ����� ���� �� ���� ���߿� ������ �гθ� �巡�� ����
	if (_isDragging)
	{	
		// �� ���� �巡�� ������ â�� ������ �� �ڽ��� â�� �巡�� ����
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

	// �κ��丮 â �巡��
	if (IsMouseInRect(rect))
	{
		// �巡�� ����
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialMousePos = _mousePos;
			//
			_offsetX = (int)_mousePos.x - (int)rect.left;
			_offsetY = (int)_mousePos.y - (int)rect.top;
			_isDragging = true;
		}
	}

	// �巡�� �� ��ġ ������Ʈ
	if (_isDragging)
	{
		// Ŭ���� ��ġ ���� �̵�
		int newLeft = _mousePos.x - _offsetX;
		int newTop = _mousePos.y - _offsetY;

		// �̵��� ��� (�巡�׷� ���� ����)
		int deltaX = newLeft - rect.left;
		int deltaY = newTop - rect.top;

		// rect�� _pos ������Ʈ
		rect.left = newLeft;
		rect.top = newTop;
		rect.right = newLeft + width;
		rect.bottom = newTop + height;
		_pos.x = newLeft;
		_pos.y = newTop;

		auto parent = static_pointer_cast<Panel>(shared_from_this());

		if (parent)
		{
			// �ڽ� ��ġ ������Ʈ
			UpdateChildPos(parent, deltaX, deltaY);
		}

		//for (auto& child : _children)
		//{
		//	Vec2 childPos = child->GetPos();
		//	child->SetPos({ childPos.x + deltaX, childPos.y + deltaY });
		//}
	}

	// �巡�� ����
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
	{
		_isDragging = false;
	}
}

void Panel::SetRelativePos(Vec2Int pos)
{
	// �θ��� �г� �߾� ����
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
		// �߰� ��Ҹ� ������ �̵�
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

					// �� �簢���� ��ġ���� ���θ� �Ǵ�
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
	// �� �簢���� ��ġ���� ���θ� �Ǵ�
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
	// �⺻ ��ġ�� ����
	_pos = _initialPos;

	for (auto& child : _children)
	{
		auto pos = child->GetInitialPos();
		child->SetPos(pos);

		auto panel = dynamic_pointer_cast<Panel>(child);

		// panel�̸鼭 �ڽ��� �ִ� ��� �ڽĿ� ���� ResetPos ����
		if (panel && panel->GetChildren().size() > 0)
			panel->ResetPos();
	}
}
