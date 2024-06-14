#include "pch.h"
#include "TitleScene.h"
#include "Sprite.h"
#include "UI.h"
#include "Button.h"
#include "TextBox.h"
#include "Panel.h"
#include "AlertBox.h"
#include "SettingPanel.h"
#include "TimeManager.h"
#include "NetworkManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SoundManager.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	// 모든 리소스는 Scene에서 불러온다
	// 다른 Scene으로 넘어갈 때 제거(Clear)한다
	// 맵 텍스쳐
	GET_SINGLE(ResourceManager)->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");

	// UI 텍스쳐
	GET_SINGLE(ResourceManager)->LoadTexture(L"Pop", L"Sprite\\UI\\pop.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"PopBackground", L"Sprite\\UI\\PopBackground.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"Title", L"Sprite\\UI\\Title.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Buttons", L"Sprite\\UI\\Buttons.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Chat", L"Sprite\\UI\\Chat.bmp");
	GET_SINGLE(ResourceManager)->LoadTexture(L"PressToStart", L"Sprite\\UI\\PressToStart.bmp", RGB(128, 128, 128));
	GET_SINGLE(ResourceManager)->LoadTexture(L"Settings", L"Sprite\\UI\\Settings.bmp", RGB(128, 128, 128));

	// 맵 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Stage01", GET_SINGLE(ResourceManager)->GetTexture(L"Stage01"));

	// UI 스프라이트
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Exit_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 130, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Option_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 30, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Option_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 230, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Option_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 430, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 630, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_Hovered", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 830, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Start_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 1030, 730, 140, 40);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Cross_Off", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 84, 634, 32, 32);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Cross_On", GET_SINGLE(ResourceManager)->GetTexture(L"Buttons"), 184, 634, 32, 32);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Pop", GET_SINGLE(ResourceManager)->GetTexture(L"Pop"), 0, 0, 130, 28);
	GET_SINGLE(ResourceManager)->CreateSprite(L"PopBackground", GET_SINGLE(ResourceManager)->GetTexture(L"PopBackground"), 0, 0, 300, 250);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Title", GET_SINGLE(ResourceManager)->GetTexture(L"Title"), 0, 0, 500, 203);
	GET_SINGLE(ResourceManager)->CreateSprite(L"Chat", GET_SINGLE(ResourceManager)->GetTexture(L"Chat"), 0, 0, 130, 28);
	GET_SINGLE(ResourceManager)->CreateSprite(L"PressToStart", GET_SINGLE(ResourceManager)->GetTexture(L"PressToStart"), 0, 0, 450, 100);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SoundSettings", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 50, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"BGM", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 75, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SFX", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60, 100, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"UI", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 60,125, 83, 25);
	GET_SINGLE(ResourceManager)->CreateSprite(L"SliderBackground", GET_SINGLE(ResourceManager)->GetTexture(L"Settings"), 145, 50, 143, 12);

	LoadPlayer();
	LoadUI();
	LoadSound();

	Super::Init();
}

void TitleScene::Update()
{
	if (this == nullptr)
		return;

	Super::Update();

	srand(time(0));

	// 배경화면 스크롤, 페이드 인 / 아웃
	BackgroundFadeInAndOut();
	
	// 아무 키나 눌러서 시작
	PressAnyToStart();

	if (_backgroundAlpha >= 250)
	{
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);
	}

	if (_uis.empty())
		return;

	for (auto& ui : _uis)
	{
		if (_sceneChanged == true)
			return;

		if (_uis.size() > 10000)
			return;

		if (ui == nullptr)
			return;

		auto setting = dynamic_pointer_cast<SettingPanel>(ui);
		if (setting == nullptr)
			continue;

		// 세팅 창 활성화 시 버튼 상호작용 불가
		bool isSettingVisible = setting->GetVisible();
		for (auto& ui : _uis)
		{
			if (ui == nullptr)
				continue;

			auto button = dynamic_pointer_cast<Button>(ui);
			if (button)
			{
				button->SetPause(isSettingVisible);
			}
		}
	}
}

void TitleScene::Render(HDC hdc)
{
	{
		// 검은 배경
		::StretchBlt(hdc,
			0,
			0,
			800,
			600,
			_black->GetDC(),
			0,
			0,
			100,
			100,
			SRCCOPY);
	}
	{
		// 백그라운드
		BLENDFUNCTION bf = {};
		bf.AlphaFormat = 0; //일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER; //  원본과 대상 이미지를 합침
		bf.SourceConstantAlpha = _alpha; // 투명도(투명 0 - 불투명 255)

		::AlphaBlend(hdc,
			0,
			0,
			800,
			600,
			_background->GetDC(),
			(_randX + _dir1 * (_sumTime * _multiplier)),
			(_randY + _dir2 * (_sumTime * _multiplier)),
			800,
			600,
			bf);
	}
	{
		// 타이틀
		int sizeX = _title->GetSize().x;
		int sizeY = _title->GetSize().y;

		::TransparentBlt(hdc,
			(GWinSizeX / 2) - (sizeX / 2),
			(GWinSizeY / 4) - (sizeY / 2),
			sizeX,
			sizeY,
			_title->GetDC(),
			0,
			0,
			sizeX,
			sizeY,
			_title->GetTransparent());
	}

	if (_isPressedAnyKey == false)
	{
		// Press Any Key to start
		int sizeX = _pressToStart->GetSize().x;
		int sizeY = _pressToStart->GetSize().y;

		::TransparentBlt(hdc,
			(GWinSizeX / 2) - (sizeX / 2),
			3 * (GWinSizeY / 4) - (sizeY / 2),
			sizeX,
			sizeY,
			_pressToStart->GetDC(),
			0,
			0,
			sizeX,
			sizeY,
			_pressToStart->GetTransparent());
	}

	Super::Render(hdc);

	if (_sceneChanged)
	{
		// 페이드용 검은 화면
		BLENDFUNCTION bf = {};
		bf.AlphaFormat = 0; //일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER; //  원본과 대상 이미지를 합침
		bf.SourceConstantAlpha = _backgroundAlpha; // 투명도(투명 0 - 불투명 255)

		::AlphaBlend(hdc,
			0,
			0,
			800,
			600,
			_black->GetDC(),
			0,
			0,
			100,
			100,
			bf);

		_backgroundAlpha += 1;
	}
}

void TitleScene::LoadPlayer()
{
}

void TitleScene::LoadUI()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Stage01");
	_title = GET_SINGLE(ResourceManager)->GetSprite(L"Title");
	_black = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");
	_pressToStart = GET_SINGLE(ResourceManager)->GetSprite(L"PressToStart");

	_randX = rand() % 2224;
	_randY = rand() % 1464;

	{
		shared_ptr<Button> exit = make_shared<Button>();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 580, 485 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &TitleScene::OnClickExitButton);
		exit->SetVisible(false);
		AddUI(exit);
	}
	{
		shared_ptr<Button> option = make_shared<Button>();
		option->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Option_Off"), BS_Default);
		option->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Option_On"), BS_Pressed);
		option->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Option_Hovered"), BS_Hovered);
		option->SetPos(Vec2{ 400, 485 });
		option->SetSize({ 140, 40 });
		option->AddOnClickDelegate(this, &TitleScene::OnClickOptionButton);
		option->SetVisible(false);
		AddUI(option);
	}
	{
		shared_ptr<Button> start = make_shared<Button>();
		start->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_Off"), BS_Default);
		start->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_On"), BS_Pressed);
		start->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_Hovered"), BS_Hovered);
		start->SetPos(Vec2{ 220, 485 });
		start->SetSize({ 140, 40 });
		start->AddOnClickDelegate(this, &TitleScene::OnClickStartButton);
		start->SetVisible(false);
		AddUI(start);
	}
	{
		auto settings = make_shared<SettingPanel>();
		settings->SetVisible(false);
		AddUI(settings);
	}
}

void TitleScene::LoadSound()
{	
	GET_SINGLE(ResourceManager)->LoadSound(L"Title_BGM", L"Sound\\Title_BGM.wav", SoundType::BGM);
	GET_SINGLE(ResourceManager)->LoadSound(L"Button", L"Sound\\Button.wav", SoundType::Effect);
	GET_SINGLE(SoundManager)->Play(L"Title_BGM", true);
}

void TitleScene::BackgroundFadeInAndOut()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += deltaTime;

	if (_isFadedIn)
	{
		_alpha += 1;

		if (_alpha >= 255)
			_isFadedIn = false;
	}
	else
	{
		if (_randX + _sumTime * _multiplier >= 2024
			|| _randY + _sumTime * _multiplier >= 1264
			|| _sumTime >= 8.f)
		{
			_alpha -= 1;

			if (_alpha <= 10)
			{
				_randX = rand() % 1200;
				_randY = rand() % 1000;

				_sumTime = 0.f;

				// 0, 1 둘 중 하나의 방향으로 진행
				_dir1 = rand() % 2;
				_dir2 = rand() % 2;

				if (_dir1 == 0 && _dir2 == 0)
				{
					_dir1 = 1;
				}

				_isFadedIn = true;
			}
		}
	}
}

void TitleScene::PressAnyToStart()
{
	// 키가 눌리면 중지
	if (_isPressedAnyKey == false)
	{
		if (GET_SINGLE(InputManager)->IsPressedAnyKey())
		{
			_isPressedAnyKey = true;

			// 버튼들 활성화
			SetChildVisible<Button>(true);

			GET_SINGLE(SoundManager)->Play(L"Button");
		}
	}
}

void TitleScene::OnClickExitButton()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd)
	{
		DestroyWindow(hWnd);
	}
}

void TitleScene::OnClickOptionButton()
{
	SetChildVisible<SettingPanel>(true);
}

void TitleScene::OnClickStartButton()
{
	// 페이드 아웃
	_sceneChanged = true;
}
