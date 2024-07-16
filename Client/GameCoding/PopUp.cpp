#include "pch.h"
#include "DevScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "PopUp.h"
#include "Button.h"

PopUp::PopUp()
{
}

PopUp::~PopUp()
{
}

void PopUp::BeginPlay()
{
	Super::BeginPlay();
}

void PopUp::Tick()
{
	Super::Tick();

	// 팝업창 닫기
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::ESC))
	{
		OnClickDenyButton();
	}
}

void PopUp::Render(HDC hdc)
{
	Super::Render(hdc);
}

void PopUp::SetVisible(bool visible)
{
	UI::SetVisible(visible);

	//if (visible == false)
	//	ResetPos();
}

shared_ptr<Button> PopUp::MakeAcceptButton()
{
	shared_ptr<Button> accept = make_shared<Button>();
	_buttonsCount++;

	accept->SetPos({ _pos.x + (_size.x / 2) - 40, _pos.y + _size.y - 35 });
	accept->SetSize({ 50,30 });
	accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Default);
	accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Hovered);
	accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopAcceptButton"), ButtonState::BS_Clicked);
	accept->AddOnClickDelegate(this, &PopUp::OnClickAcceptButton);
	accept->SetInitialPos(accept->GetPos());
	accept->SetButtonName(L"accept");
	AddChild(accept);

	return accept;
}

shared_ptr<Button> PopUp::MakeDenyButton()
{
	shared_ptr<Button> deny = make_shared<Button>();
	_buttonsCount++;

	deny->SetPos({ _pos.x + (_size.x / 2) + 40, _pos.y + _size.y - 35 });
	deny->SetSize({ 50,30 });
	deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Default);
	deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Hovered);
	deny->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"PopDenyButton"), ButtonState::BS_Clicked);
	deny->AddOnClickDelegate(this, &PopUp::OnClickDenyButton);
	deny->SetInitialPos(deny->GetPos());
	deny->SetButtonName(L"deny");
	AddChild(deny);

	return deny;
}

void PopUp::OnClickAcceptButton()
{
	_parent->SetPause(false);

	// 부모 UI에서 작동할 콜백 함수가 있으면 기능 수행
	if (_parentCallback != nullptr)
	{
		_parentCallback();
	}

	auto panel = static_pointer_cast<Panel>(_parent);
	if (panel)
	{
		auto ui = dynamic_pointer_cast<UI>(shared_from_this());

		if (ui)
			panel->RemoveChild(ui);
	}
}

void PopUp::OnClickDenyButton()
{
	_parent->SetPause(false);

	auto panel = static_pointer_cast<Panel>(_parent);
	if (panel)
	{
		auto ui = dynamic_pointer_cast<UI>(shared_from_this());

		if (ui)
			panel->RemoveChild(ui);
	}
}