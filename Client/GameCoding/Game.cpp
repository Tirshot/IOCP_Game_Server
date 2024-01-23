#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "ChatManager.h"

Game::Game()
{
	
}

Game::~Game()
{
	GET_SINGLE(SceneManager)->Clear();
	GET_SINGLE(ResourceManager)->Clear();
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // _hdc와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 비트맵 연결
	::DeleteObject(prev);

	filesystem::path currentPath = filesystem::current_path();
	filesystem::path resourceDirectory = currentPath / ".." / ".." / "Client" / "Resources";
	filesystem::path relativePath = filesystem::relative(resourceDirectory, currentPath);
	
	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->Init();
	GET_SINGLE(ResourceManager)->Init(hwnd, fs::path(relativePath));
	GET_SINGLE(SoundManager)->Init(hwnd);

	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);

	GET_SINGLE(NetworkManager)->Init();
}

void Game::Update()
{
	srand(unsigned(time(0)));
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
	GET_SINGLE(NetworkManager)->Update();
	GET_SINGLE(ChatManager)->Tick();
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);

	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	{ // 마우스 위치 출력
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		SetTextColor(_hdcBack, RGB(0, 0, 0));
		SetBkMode(_hdcBack, TRANSPARENT);
		::TextOut(_hdcBack, 680, 28, str.c_str(), static_cast<int32>(str.size()));
	}

	{ // FPS, Delta Time 출력
		wstring str = std::format(L"FPS : {0}", fps);
		SetTextColor(_hdcBack, RGB(0, 0, 0));
		SetBkMode(_hdcBack, TRANSPARENT);
		::TextOut(_hdcBack, 680, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// 더블 버퍼링, 비트 블릿 ; 고속 복사
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);

	// 복사 이후 백 버퍼 클리어
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);

}
