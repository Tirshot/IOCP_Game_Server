#include "pch.h"
#include "AlertBox.h"
#include "Sprite.h"
#include "UI.h"
#include "TextBox.h"
#include "Button.h"
#include "InputManager.h"
#include "ResourceManager.h"

AlertBox::AlertBox()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");
	_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");

	// 텍스트
	{
		TextBox* text = new TextBox();
		text->SetText(L"테스트용 텍스트입니다. \n두번째 줄입니다. \n 세번째 줄입니다.");
		text->SetPos(Vec2{_pos.x + 30, _pos.y + 10});
		text->SetSize(Vec2Int{ _size.x - 30, _size.y - 40 });
		AddChild(text);
	}

	// 확인
	{
		Button* accept = new Button();
		//accept->SetSprite(, ButtonState::BS_Default);
		//accept->SetSprite(, ButtonState::BS_Hovered);
		//accept->SetSprite(, ButtonState::BS_Clicked);
		auto parent = GetParent();
		accept->AddOnClickDelegate(this, AlertBox::OnClickAcceptButton);
		//AddChild(accept);
	}

	// 취소
	{
		Button* deny = new Button();
		//deny->SetSprite(, ButtonState::BS_Default);
		//deny->SetSprite(, ButtonState::BS_Hovered);
		//deny->SetSprite(, ButtonState::BS_Clicked);
		//AddChild(deny);
	}
}

AlertBox::~AlertBox()
{
}

void AlertBox::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void AlertBox::Tick()
{
	for (auto& child : _children)
		child->Tick();
}

void AlertBox::Render(HDC hdc)
{
	::TransparentBlt(hdc,
		_pos.x + 10,
		_pos.y + 10,
		43,
		43,
		_icon->GetDC(),
		_icon->GetPos().x,
		_icon->GetPos().y,
		_icon->GetSize().x,
		_icon->GetSize().y,
		_icon->GetTransparent());

	for (auto& child : _children)
		if (child->GetVisible() == true)
			child->Render(hdc);
}

void AlertBox::SetIcon(wstring wstr)
{
	if (wstr == L"Danger")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"DangerIcon");
	}
	else if (wstr == L"Alert")
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"AlertIcon");
	}
	else
	{
		_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
	}
}

void AlertBox::OnClickAcceptButton()
{
	_visible = false;
}
