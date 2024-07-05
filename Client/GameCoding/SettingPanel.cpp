#include "pch.h"
#include "SettingPanel.h"
#include "Sprite.h"
#include "Button.h"
#include "SliderBar.h"
#include "DevScene.h"
#include "TitleScene.h"
#include "ResourceManager.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "SceneManager.h"

SettingPanel::SettingPanel()
{

}

SettingPanel::~SettingPanel()
{

}

void SettingPanel::BeginPlay()
{
	// 500 x 360
	SetSize({ 500, 450 });
	SetPos({ 150, 70 });

	_rect.left = _pos.x;
	_rect.top = _pos.y;
	_rect.right = _pos.x + _size.x;
	_rect.bottom = _pos.y + _size.y;

	_background = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
	_soundSettings = GET_SINGLE(ResourceManager)->GetSprite(L"SoundSettings");
	_BGM = GET_SINGLE(ResourceManager)->GetSprite(L"BGM");
	_UI = GET_SINGLE(ResourceManager)->GetSprite(L"UI");
	_SFX = GET_SINGLE(ResourceManager)->GetSprite(L"SFX");

	{
		// 닫기 버튼
		shared_ptr<Button> cross = make_shared<Button>();
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
		auto slider = make_shared<SliderBar>();
		auto value = GET_SINGLE(SoundManager)->GetMasterVolume();
		slider->SetPos({ _pos.x + 280,_pos.y + 120 });
		slider->SetSize({200, 15});
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetValue(value);
		slider->SetName(L"ALL");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetAllVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		auto slider = make_shared<SliderBar>();
		slider->SetPos({ _pos.x + 280,_pos.y + 170 });
		slider->SetSize({ 200, 15 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetValue(GET_SINGLE(SoundManager)->GetBGMVolume());
		slider->SetName(L"BGM");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetBGMVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		auto slider = make_shared<SliderBar>();
		slider->SetPos({ _pos.x + 280,_pos.y + 220 });
		slider->SetSize({ 200, 15 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetValue(GET_SINGLE(SoundManager)->GetSFXVolume());
		slider->SetName(L"SFX");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetSFXVolume);
		AddChild(slider);
	}
	{
		// 슬라이더
		auto slider = make_shared<SliderBar>();
		slider->SetPos({ _pos.x + 280,_pos.y + 270 });
		slider->SetSize({ 200, 15 });
		slider->SetMaximumValue(1);
		slider->SetMinimumValue(0);
		slider->SetValue(GET_SINGLE(SoundManager)->GetUIVolume());
		slider->SetName(L"UI");
		slider->AddOnSlideDelegate(this, &SettingPanel::SetUIVolume);
		AddChild(slider);
	}

	{ // 게임 종료
		shared_ptr<Button> exit = make_shared<Button>();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ GWinSizeX / 2, (float)_size.y });
		exit->SetSize({ 140, 40 });
		exit->SetVisible(true);
		exit->AddOnClickDelegate(this, &SettingPanel::OnClickExitButton);
		AddChild(exit);
	}

	Super::BeginPlay();
}

void SettingPanel::Tick()
{
	Super::Tick();

	static auto scene = GET_SINGLE(SceneManager)->GetDevScene();

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

	Super::Render(hdc);
}

void SettingPanel::OnClickCrossButton()
{
	SetVisible(false);
}

void SettingPanel::OnClickExitButton()
{
	auto scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	if (scene)
	{
		auto titleScene = dynamic_pointer_cast<TitleScene>(scene);
		if (titleScene)
		{
			SetVisible(false);
			return;
		}
	}

	// 수행 후 ui 제거
	auto devScene = GET_SINGLE(SceneManager)->GetDevScene();
	if (devScene)
	{
		devScene->RemoveUI(shared_from_this());

		auto myPlayerID = GET_SINGLE(SceneManager)->GetMyPlayerId();

		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_LeaveGame(myPlayerID);
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	GET_SINGLE(NetworkManager)->RemoveSession();
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::TitleScene);
}

void SettingPanel::SetAllVolume()
{
	for (auto& child : _children)
	{
		auto slider = dynamic_pointer_cast<SliderBar>(child);
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
		auto slider = dynamic_pointer_cast<SliderBar>(child);
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
		auto slider = dynamic_pointer_cast<SliderBar>(child);
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
		auto slider = dynamic_pointer_cast<SliderBar>(child);
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