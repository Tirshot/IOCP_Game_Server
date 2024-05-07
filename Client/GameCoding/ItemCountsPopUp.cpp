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
	// ����� ��ġ�� �߾ӿ� ��ġ��Ű�� ���ؼ��� Size ������ Pos �������� ����!!
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };
	_initialPos = _pos;
	// �ؽ�Ʈ
	{
		TextBox* text = new TextBox();
		text->SetText(L"�׽�Ʈ�� �ؽ�Ʈ�Դϴ�. \n�ι�° ���Դϴ�. \n ����° ���Դϴ�.");
		text->SetPos(Vec2{ _pos.x + 60, _pos.y + 20 });
		text->SetSize(Vec2Int{ _size.x - 80, (_size.y / 2) - 25 });
		text->SetPadding(3, 3);
		text->SetInitialPos(text->GetPos());
		AddChild(text);
	}

	// Ȯ��
	{
		Button* accept = new Button();
		accept->SetPos({ _pos.x + _size.x / 2 - 40, _pos.y + _size.y - 30 });
		accept->SetSize({ 50,30 });
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Hovered);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Clicked);
		accept->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickAcceptButton);
		accept->SetInitialPos(accept->GetPos());
		AddChild(accept);
	}

	// ���
	{
		Button* deny = new Button();
		deny->SetPos({ _pos.x + _size.x / 2 + 40, _pos.y + _size.y - 30 });
		deny->SetSize({ 50,30 });
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Default);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Hovered);
		deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Clicked);
		deny->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickDenyButton);
		deny->SetInitialPos(deny->GetPos());
		AddChild(deny);
	}

	// ������ ���� ����
	{
		Button* minus = new Button();
		minus->SetPos({ _pos.x + _size.x / 2 - 34, _pos.y + _size.y - 55 });
		minus->SetSize({ 30,18 });
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Default);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Hovered);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Clicked);
		minus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountMinusButton);
		minus->SetInitialPos(minus->GetPos());
		AddChild(minus);
	}

	// ������ ���� 10�� ����
	{
		Button* doubleMinus = new Button();
		doubleMinus->SetPos({ _pos.x + _size.x / 2 - 78, _pos.y + _size.y - 55 });
		doubleMinus->SetSize({ 30,18 });
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Default);
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Hovered);
		doubleMinus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LLButton"), ButtonState::BS_Clicked);
		doubleMinus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountDoubleMinusButton);
		doubleMinus->SetInitialPos(doubleMinus->GetPos());
		AddChild(doubleMinus);
	}

	// ������ ���� ����
	{
		Button* plus = new Button();
		plus->SetPos({ _pos.x + _size.x / 2 + 42, _pos.y + _size.y - 55 });
		plus->SetSize({ 30,18 });
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Default);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Hovered);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Clicked);
		plus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountPlusButton);
		plus->SetInitialPos(plus->GetPos());
		AddChild(plus);
	}

	// ������ ���� 10�� ����
	{
		Button* doublePlus = new Button();
		doublePlus->SetPos({ _pos.x + _size.x / 2 + 86, _pos.y + _size.y - 55 });
		doublePlus->SetSize({ 30,18 });
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Default);
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Hovered);
		doublePlus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RRButton"), ButtonState::BS_Clicked);
		doublePlus->AddOnClickDelegate(this, &ItemCountsPopUp::OnClickCountDoublePlusButton);
		doublePlus->SetInitialPos(doublePlus->GetPos());
		AddChild(doublePlus);
	}

	for (auto& child : _children)
		child->BeginPlay();
}

void ItemCountsPopUp::Tick()
{
	_rect = { (int)_pos.x , (int)_pos.y, (int)_pos.x + (_size.x), (int)_pos.y + (_size.y) };

	Panel::DragAndMove(&_rect);

	_totalPrice = _price * _counts;

	for (auto& child : _children)
		if (_visible == true)
			child->Tick();
}

void ItemCountsPopUp::Render(HDC hdc)
{
	if (_visible == false)
		return;

	// ���
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

	// ������ ����
	{
		wstring itemCount = to_wstring(_counts) + L"��";
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2) + 25,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, itemCount.c_str(), -1, &_textRect, DT_CENTER);
	}

	// ��� ������
	{
		Sprite* goldImage = GET_SINGLE(ResourceManager)->GetSprite(L"Gold");

		if (goldImage)
		::TransparentBlt(hdc,
			_pos.x + 115,
			_pos.y + 85,
			goldImage->GetSize().x,
			goldImage->GetSize().y,
			goldImage->GetDC(),
			0,
			0,
			goldImage->GetSize().x,
			goldImage->GetSize().y,
			goldImage->GetTransparent());
	}
	// ���� ���
	{
		wstring totalPrice = to_wstring(_totalPrice);
		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2),_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, totalPrice.c_str(), -1, &_textRect, DT_CENTER);
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

	// �θ� UI���� �۵��� �ݹ� �Լ��� ������ ��� ����
	if (_parentCallback != nullptr)
	{
		_parentCallback();
	}

	ResetPos();
	_counts = 1;
	_price = 0;
}

void ItemCountsPopUp::OnClickDenyButton()
{
	SetVisible(false);

	ResetPos();
	_counts = 1;
	_price = 0;
}

void ItemCountsPopUp::OnClickCountPlusButton()
{
	_counts++;
}

void ItemCountsPopUp::OnClickCountDoublePlusButton()
{
	_counts += 10;

	if (_counts > 99)
		_counts = 99;
}

void ItemCountsPopUp::OnClickCountMinusButton()
{
	_counts--;

	if (_counts <= 1)
		_counts = 1;
}

void ItemCountsPopUp::OnClickCountDoubleMinusButton()
{
	_counts -= 10;

	if (_counts <= 1)
		_counts = 1;
}

void ItemCountsPopUp::SetText(wstring wstr)
{
	auto textbox = FindChild<TextBox>(_children);
	
	if (textbox)
		textbox->SetText(wstr);
}
