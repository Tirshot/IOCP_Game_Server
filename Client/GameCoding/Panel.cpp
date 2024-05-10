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
	// �ش��ϴ� ui�� ã�� ���ߴٸ�
	if (findIt == _children.end())
		return false;

	_children.erase(findIt);
	return true;
}

void Panel::UpdateChildPos(Panel* parent, int deltaX, int deltaY)
{
	// �ڽ� ��ġ ������Ʈ
	if (parent)
		for (auto& child : parent->GetChildren())
		{
			if (child->GetParent() != parent)
				continue;

			// AlertBox�� ���� �����̴� ���� ����
			if (dynamic_cast<AlertBox*>(child))
				continue;

			// ItemCountsPopUp�� ���� �����̴� ���� ����
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

	// ���� ���� �г��� ����� ���� �� ���� ���߿� ������ �гθ� �巡�� ����
	if (_isDragging)
	{	
		// �� ���� �巡�� ������ â�� ������ �� �ڽ��� â�� �巡�� ����
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

	// �κ��丮 â �巡��
	if (IsMouseInRect(*rect))
	{
		// �巡�� ����
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialMousePos = _mousePos;
			//
			_offsetX = (int)_mousePos.x - (int)rect->left;
			_offsetY = (int)_mousePos.y - (int)rect->top;
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
		int deltaX = newLeft - rect->left;
		int deltaY = newTop - rect->top;

		// rect�� _pos ������Ʈ
		rect->left = newLeft;
		rect->top = newTop;
		rect->right = newLeft + width;
		rect->bottom = newTop + height;
		_pos.x = newLeft;
		_pos.y = newTop;

		// �ڽ� ��ġ ������Ʈ
		UpdateChildPos(this, deltaX, deltaY);
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

bool Panel::IsOverlapped(RECT& thisRect, RECT& other)
{
	// �� �簢���� ��ġ���� ���θ� �Ǵ�
	return !(thisRect.right < other.left || thisRect.left > other.right ||
		thisRect.bottom < other.top || thisRect.top > other.bottom);
}

void Panel::ResetPos()
{
	// �⺻ ��ġ�� ����
	_pos = _initialPos;

	for (auto& child : _children)
	{
		auto pos = child->GetInitialPos();
		child->SetPos(pos);

		auto panel = dynamic_cast<Panel*>(child);

		// panel�̸鼭 �ڽ��� �ִ� ��� �ڽĿ� ���� ResetPos ����
		if (panel && panel->GetChildren().size() > 0)
			panel->ResetPos();
	}
}
