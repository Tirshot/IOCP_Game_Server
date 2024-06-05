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
}

AlertBox::~AlertBox()
{
}

void AlertBox::BeginPlay()
{
	// 기즈모 위치를 중앙에 위치시키기 위해서는 Size 설정을 Pos 설정보다 먼저!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	// 텍스트
	{
		TextBox* text = new TextBox();
		text->SetText(L"테스트용 텍스트입니다. \n두번째 줄입니다. \n 세번째 줄입니다.");
		text->SetPos(Vec2{ _pos.x + 60, _pos.y  + 10 });
		text->SetSize(Vec2Int{ (_size.x / 4) * 3, (_size.y / 2) });
		text->SetInitialPos(text->GetPos());
		AddChild(text);
	}

	// 확인
	{
		Button* accept = new Button();
		accept->SetPos({ _pos.x + _size.x / 2 - 40, _pos.y + _size.y - 35});
		accept->SetSize({ 50,30 });
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Hovered);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Clicked);
		accept->AddOnClickDelegate(this, &AlertBox::OnClickAcceptButton);
		accept->SetInitialPos(accept->GetPos());
		AddChild(accept);
	}

	// 취소
	{
		Button* deny = new Button();
		deny->SetPos({ _pos.x + _size.x  / 2 + 40, _pos.y + _size.y  - 35 });
		deny->SetSize({ 50,30 });
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Default);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Hovered);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Clicked);
		deny->AddOnClickDelegate(this, &AlertBox::OnClickDenyButton);
		deny->SetInitialPos(deny->GetPos());
		AddChild(deny);
	}

	_initialPos = _pos;

	for (auto& child : _children)
		child->BeginPlay();
}

void AlertBox::Tick()
{
	// 기즈모 위치를 중앙에 위치시키기 위해서는 Size 설정을 Pos 설정보다 먼저!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	Panel::DragAndMove(&_rect);

	for (auto& child : _children)
		if (_visible == true)
			child->Tick();
}

void AlertBox::Render(HDC hdc)
{
	if (_visible == false)
	{
		return;
	}

	// 배경
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

	for (auto& child : _children)
		if (child->GetVisible() == true)
			child->Render(hdc);

	if (_icon == nullptr)
		return;

	// 경고 아이콘
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

void AlertBox::OnClickAcceptButton()
{
	SetVisible(false);

	ResetPos();

	// 부모 UI에서 작동할 콜백 함수가 있으면 기능 수행
	if (_parentCallback != nullptr)
	{
		_parentCallback();
	}
}

void AlertBox::OnClickDenyButton()
{
	SetVisible(false);

	ResetPos();
}

void AlertBox::SetText(wstring wstr)
{
	auto textbox = FindChild<TextBox>(_children);
	textbox->SetText(wstr);
}
