#include "pch.h"
#include "Game.h"
#include "TitleScene.h"
#include "DevScene.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "QuestManager.h"

Game::Game()
{
	
}

Game::~Game()
{
	//GET_SINGLE(SceneManager)->Clear();
	//GET_SINGLE(ResourceManager)->Clear();
	//GET_SINGLE(ItemManager)->Clear();
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc�� ȣȯ�Ǵ� DC�� ����
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // _hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC�� ��Ʈ�� ����
	::DeleteObject(prev);

	filesystem::path currentPath = filesystem::current_path();
	filesystem::path resourceDirectory = currentPath / ".." / ".." / "Client" / "Resources";
	filesystem::path relativePath = filesystem::relative(resourceDirectory, currentPath);
	filesystem::path Path = "E:\\Cpp\\IOCP\\Server\\Client\\Resources";

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd);

	GET_SINGLE(ResourceManager)->Init(hwnd, fs::path(Path));
	GET_SINGLE(SoundManager)->Init(hwnd);

	GET_SINGLE(SceneManager)->Init();
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::TitleScene);

}

void Game::Update()
{
	srand(unsigned(time(0)));
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();

	auto devScene = dynamic_pointer_cast<DevScene>(GET_SINGLE(SceneManager)->GetCurrentScene());

	if (devScene)
	{
		if (_initialized == false)
		{
			GET_SINGLE(ItemManager)->Init();
			GET_SINGLE(NetworkManager)->Init();
			GET_SINGLE(QuestManager)->BeginPlay();
			_initialized = true;
		}

		GET_SINGLE(NetworkManager)->Update();
		GET_SINGLE(QuestManager)->Tick();
		GET_SINGLE(ChatManager)->Tick();
	}
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);

	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	// �۲� ����
	static HFONT hFont = CreateFont(
		-MulDiv(10, GetDeviceCaps(_hdc, LOGPIXELSY), 72), // (16 * DPI) / 72 (1��ġ�� ����Ʈ)
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, L"Malgun Gothic"
	);

	// ���� ���
	HFONT hOldFont = (HFONT)SelectObject(_hdcBack, hFont);
	SelectObject(_hdc, hOldFont);

	{ // FPS, Delta Time ���
		wstring str = std::format(L"FPS : {0}", fps);
		SetTextColor(_hdcBack, RGB(255, 255, 255));
		SetBkMode(_hdcBack, TRANSPARENT);
		::TextOut(_hdcBack, 5, 560, str.c_str(), static_cast<int32>(str.size()));
	}

	{ // ���콺 ��ġ ���
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		SetTextColor(_hdcBack, RGB(255, 255, 255));
		SetBkMode(_hdcBack, TRANSPARENT);
		::TextOut(_hdcBack, 5, 578, str.c_str(), static_cast<int32>(str.size()));
	}

	// ���� ���۸�, ��Ʈ �� ; ��� ����
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);

	// ���� ���� �� ���� Ŭ����
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void Game::Restart()
{
	GET_SINGLE(SceneManager)->Clear();
	GET_SINGLE(ResourceManager)->Clear();
	GET_SINGLE(ItemManager)->Clear();

	Init(_hwnd);
}
