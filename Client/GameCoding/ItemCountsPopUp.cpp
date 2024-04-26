#include "pch.h"
#include "ItemCountsPopUp.h"
#include "Sprite.h"
#include "UI.h"
#include "TextBox.h"
#include "Button.h"
#include "InputManager.h"
#include "ItemManager.h"
#include "ResourceManager.h"

ItemCountsPopUp::ItemCountsPopUp()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Pop");
	_icon = GET_SINGLE(ResourceManager)->GetSprite(L"InformationIcon");
}

ItemCountsPopUp::~ItemCountsPopUp()
{

}

void ItemCountsPopUp::BeginPlay()
{
	// 기즈모 위치를 중앙에 위치시키기 위해서는 Size 설정을 Pos 설정보다 먼저!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };
	_initialPos = _pos;
	// 텍스트
	{
		TextBox* text = new TextBox();
		text->SetText(L"테스트용 텍스트입니다. \n두번째 줄입니다. \n 세번째 줄입니다.");
		text->SetPos(Vec2{ _pos.x + 60, _pos.y + 10 });
		text->SetSize(Vec2Int{ _size.x - 80, (_size.y / 2) });
		AddChild(text);
	}

	// 확인
	{
		Button* accept = new Button();
		accept->SetPos({ _pos.x + _size.x / 2 - 40, _pos.y + _size.y - 30 });
		accept->SetSize({ 50,30 });
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Hovered);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Clicked);
		accept->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickAcceptButton);
		AddChild(accept);
	}

	// 취소
	{
		Button* deny = new Button();
		deny->SetPos({ _pos.x + _size.x / 2 + 40, _pos.y + _size.y - 30 });
		deny->SetSize({ 50,30 });
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Default);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Hovered);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Clicked);
		deny->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickDenyButton);
		AddChild(deny);
	}

	// 아이템 수량 감소
	{
		Button* minus = new Button();
		minus->SetPos({ _pos.x + _size.x / 2 - 34, _pos.y + _size.y - 60 });
		minus->SetSize({ 40,24 });
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Default);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Hovered);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Clicked);
		minus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountMinusButton);
		AddChild(minus);
	}

	// 아이템 수량 증가
	{
		Button* plus = new Button();
		plus->SetPos({ _pos.x + _size.x / 2 + 42, _pos.y + _size.y - 60 });
		plus->SetSize({ 40,24 });
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Default);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Hovered);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Clicked);
		plus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountPlusButton);
		AddChild(plus);
	}

	for (auto& child : _children)
		child->BeginPlay();
}

void ItemCountsPopUp::Tick()
{
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	Panel::DragAndMove(&_rect);

	for (auto& child : _children)
		if (_visible == true)
			child->Tick();
}

void ItemCountsPopUp::Render(HDC hdc)
{
	if (_visible == false)
		return;

	// 배경
	::TransparentBlt(hdc,
		(int32)_pos.x,
		(int32)_pos.y,
		GetSize().x + 10,
		GetSize().y,
		_background->GetDC(),
		_background->GetPos().x,
		_background->GetPos().y,
		_background->GetSize().x,
		_background->GetSize().y,
		_background->GetTransparent());

	for (auto& child : _children)
		if (child->GetVisible() == true)
			child->Render(hdc);

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

	// 아이템 갯수
	{
		wstring itemCount = to_wstring(_counts);
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) + 20,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCount.c_str(), -1, &_textRect, DT_CENTER);
	}
}

void ItemCountsPopUp::SetIcon(wstring wstr)
{
	// Danger, Alert, Information
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

void ItemCountsPopUp::OnClickAcceptButton()
{
	SetVisible(false);

	// 부모 UI에서 작동할 콜백 함수가 있으면 기능 수행
	if (_parentCallback != nullptr)
	{
		_parentCallback();
	}

	_counts = 0;
	_price = 0;
}

void ItemCountsPopUp::OnClickDenyButton()
{
	SetVisible(false);

	_counts = 0;
	_price = 0;
}

void ItemCountsPopUp::OnClickCountPlusButton()
{
	_counts++;
}

void ItemCountsPopUp::OnClickCountMinusButton()
{
	_counts--;

	if (_counts <= 1)
		_counts = 1;
}

void ItemCountsPopUp::SetText(wstring wstr)
{
	auto textbox = FindChild<TextBox>(_children);
	
	if (textbox)
		textbox->SetText(wstr);
}
