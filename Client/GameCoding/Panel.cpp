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

	// �κ��丮 â �巡��
	if (IsMouseInRect(*rect))
	{
		// �巡�� ����
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_initialPos = _mousePos;
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
