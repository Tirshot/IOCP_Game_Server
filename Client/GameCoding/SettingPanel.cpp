#include "pch.h"
#include "SettingPanel.h"
#include "Sprite.h"
#include "Button.h"
#include "SliderBar.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SoundManager.h"

SettingPanel::SettingPanel()
{
	// 500 x 360
	SetSize({ 500, 450 });
	SetPos({ 150, 70 });

	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _pos.x + _size.x;
	_rect.bottom = _pos.y + _size.y;

	_soundSettings = GET_SINGLE(ResourceManager)->GetSprite(L"SoundSettings");
	_BGM = GET_SINGLE(ResourceManager)->GetSprite(L"BGM");
	_UI = GET_SINGLE(ResourceManager)->GetSprite(L"UI");
	_SFX = GET_SINGLE(ResourceManager)->GetSprite(L"SFX");
}

SettingPanel::~SettingPanel()
{

}

void SettingPanel::BeginPlay()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
	_soundSettings = GET_SINGLE(ResourceManager)->GetSprite(L"SoundSettings");
	_BGM = GET_SINGLE(ResourceManager)->GetSprite(L"BGM");
	_UI = GET_SINGLE(ResourceManager)->GetSprite(L"UI");
	_SFX = GET_SINGLE(ResourceManager)->GetSprite(L"SFX");

	{
		// 닫기 버튼
		Button* cross = new Button;
		cross->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Cross_Off"), BS_Default);
		cross->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Cross_Off"), BS_Hovered);
		cross->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Cross_On"), BS_Pressed);
		cross->SetSize({ 32, 32 });
		cross->SetPos(Vec2{ _pos.x + _size.x - (cross->GetSize().x / 2), _pos.y + (cross->GetSize().y / 2) });
		cross->AddOnClickDelegate(this, &SettingPanel::OnClickCrossButton);
		cross->SetVisible(true);
		AddChild(cross);
	}
	{
		// 슬라이더
		SliderBar* slider = new SliderBar;
		slider->SetPos({ _pos.x + 280,_pos.y + 120 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetName(L"ALL");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetAllVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		SliderBar* slider = new SliderBar;
		slider->SetPos({ _pos.x + 280,_pos.y + 170 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetName(L"BGM");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetBGMVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		SliderBar* slider = new SliderBar;
		slider->SetPos({ _pos.x + 280,_pos.y + 220 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetName(L"SFX");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetSFXVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		SliderBar* slider = new SliderBar;
		slider->SetPos({ _pos.x + 280,_pos.y + 270 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetName(L"UI");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetUIVolume);
		AddChild(slider);
	}
	for (UI* child : _children)
		child->BeginPlay();
}

void SettingPanel::Tick()
{
	for (UI* child : _children)
		child->Tick();

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::ESC))
	{
		SetVisible(false);
	}

	if (IsMouseOutRect(_rect))
	{
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			SetVisible(false);
		}
	}


}

void SettingPanel::Render(HDC hdc)
{
	::StretchBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_background->GetDC(),
		0,
		0,
		_background->GetSize().x,
		_background->GetSize().y,
		SRCCOPY
	);

	//for (auto& child : _children)
	//{
	//	auto* slider = dynamic_cast<SliderBar*>(child);
	//	if (slider)
	//	{
	//		RECT _textRect = { _pos.x + (_size.x / 2) - 10, _pos.y + (_size.y / 2),_textRect.left + 30,_textRect.top + 18 };
	//		DrawTextW(hdc, to_wstring(slider->GetValue()).c_str(), -1, &_textRect, DT_CENTER | WHITENESS);
	//		break;
	//	}
	//}

	int sizeX = _soundSettings->GetSize().x;
	int sizeY =	_soundSettings->GetSize().y;

	::TransparentBlt(hdc,
		_pos.x + 155 - (sizeX / 2),
		_pos.y + 120 - (sizeY / 2),
		sizeX,
		sizeY,
		_soundSettings->GetDC(),
		_soundSettings->GetPos().x,
		_soundSettings->GetPos().y,
		sizeX,
		sizeY,
		_soundSettings->GetTransparent());

	::TransparentBlt(hdc,
		_pos.x + 155 - (sizeX / 2),
		_pos.y + 170 - (sizeY / 2),
		sizeX,
		sizeY,
		_BGM->GetDC(),
		_BGM->GetPos().x,
		_BGM->GetPos().y,
		sizeX,
		sizeY,
		_BGM->GetTransparent());

	::TransparentBlt(hdc,
		_pos.x + 155 - (sizeX / 2),
		_pos.y + 220 - (sizeY / 2),
		sizeX,
		sizeY,
		_SFX->GetDC(),
		_SFX->GetPos().x,
		_SFX->GetPos().y,
		sizeX,
		sizeY,
		_SFX->GetTransparent());

	::TransparentBlt(hdc,
		_pos.x + 155 - (sizeX / 2),
		_pos.y + 270 - (sizeY / 2),
		sizeX,
		sizeY,
		_UI->GetDC(),
		_UI->GetPos().x,
		_UI->GetPos().y,
		sizeX,
		sizeY,
		_UI->GetTransparent());

	for (UI* child : _children)
		child->Render(hdc);
}

void SettingPanel::OnClickCrossButton()
{
	SetVisible(false);
}

void SettingPanel::SetAllVolume()
{
	for (auto& child : _children)
	{
		auto slider = dynamic_cast<SliderBar*>(child);
		if (slider)
		{
			if (slider->GetName() == L"ALL")
			{
				auto value = slider->GetValue();
				GET_SINGLE(SoundManager)->SetMasterVolume(value);
			}
		}
	}
}

void SettingPanel::SetBGMVolume()
{
	for (auto& child : _children)
	{
		auto slider = dynamic_cast<SliderBar*>(child);
		if (slider)
		{
			if (slider->GetName() == L"BGM")
			{
				auto value = slider->GetValue();
				GET_SINGLE(SoundManager)->SetBGMVolume(value);
			}
		}
	}
}

void SettingPanel::SetSFXVolume()
{
	for (auto& child : _children)
	{
		auto slider = dynamic_cast<SliderBar*>(child);
		if (slider)
		{
			if (slider->GetName() == L"SFX")
			{
				auto value = slider->GetValue();
				GET_SINGLE(SoundManager)->SetSFXVolume(value);
			}
		}
	}
}

void SettingPanel::SetUIVolume()
{
	for (auto& child : _children)
	{
		auto slider = dynamic_cast<SliderBar*>(child);
		if (slider)
		{
			if (slider->GetName() == L"UI")
			{
				auto value = slider->GetValue();
				GET_SINGLE(SoundManager)->SetUIVolume(value);
			}
		}
	}
}
