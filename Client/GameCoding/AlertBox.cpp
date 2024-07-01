#include "pch.h"
#include "AlertBox.h"
#include "Sprite.h"
#include "UI.h"
#include "TextBox.h"
#include "Button.h"
#include "Inventory.h"
#include "InputManager.h"
#include "ResourceManager.h"

AlertBox::AlertBox()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");
	_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
}

AlertBox::~AlertBox()
{
}

void AlertBox::BeginPlay()
{
	Super::BeginPlay();

	// ����� ��ġ�� �߾ӿ� ��ġ��Ű�� ���ؼ��� Size ������ Pos �������� ����!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };
	_initialPos = _pos;

	// �ؽ�Ʈ
	{
		shared_ptr<TextBox> _text = make_shared<TextBox>();
		_text->SetText(L"�׽�Ʈ�� �ؽ�Ʈ�Դϴ�. \n�ι�° ���Դϴ�. \n ����° ���Դϴ�.");
		_text->SetPos(Vec2{ _pos.x + 60, _pos.y + 10 });
		_text->SetSize(Vec2Int{ (_size.x / 4) * 3, (_size.y / 2) });
		_text->SetInitialPos(_text->GetPos());
		AddChild(_text);
	}
}

void AlertBox::Tick()
{
	Super::Tick();

	// ����� ��ġ�� �߾ӿ� ��ġ��Ű�� ���ؼ��� Size ������ Pos �������� ����!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	Panel::DragAndMove(_rect);

	for (auto& child : _children)
	{
		auto button = dynamic_pointer_cast<Button>(child);
		if (button)
		{
			switch (_buttonsCount)
			{
			case 1:
				button->SetPos({ _pos.x + (_size.x / 2), _pos.y + _size.y - 35 });
				break;

			case 2:
				if (button->GetButtonName() == L"accept")
				{
					button->SetPos({ _pos.x + (_size.x / 2) - 40, _pos.y + _size.y - 35 });
				}
				else
				{
					button->SetPos({ _pos.x + (_size.x / 2) + 40, _pos.y + _size.y - 35 });
				}
				break;
			}
		}
	}
}

void AlertBox::Render(HDC hdc)
{
	if (_visible == false)
	{
		return;
	}

	// ���
	::TransparentBlt(hdc,
		(int32)_pos.x,
		(int32)_pos.y,
		_size.x + 10,
		_size.y,
		_background->GetDC(),
		_background->GetPos().x,
		_background->GetPos().y,
		_background->GetSize().x,
		_background->GetSize().y,
		_background->GetTransparent());

	if (_icon == nullptr)
		return;

	// ��� ������
	::TransparentBlt(hdc,
		_pos.x + 10,
		_pos.y + (int)_size.y / 6,
		43,
		43,
		_icon->GetDC(),
		_icon->GetPos().x,
		_icon->GetPos().y,
		_icon->GetSize().x,
		_icon->GetSize().y,
		_icon->GetTransparent());

	Super::Render(hdc);
}

void AlertBox::SetIcon(wstring wstr)
{
	// Danger, Alert, Information
	if (wstr == L"Danger")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"DangerIcon");
	}
	else if (wstr == L"Alert" || wstr == L"Warning")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"AlertIcon");
	}
	else
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
	}
}

void AlertBox::SetText(wstring wstr)
{
	for (auto& child : _children)
	{
		auto text = dynamic_pointer_cast<TextBox>(child);
		if (text)
			text->SetText(wstr);
	}
}
