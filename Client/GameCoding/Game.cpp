#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"

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

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc�� ȣȯ�Ǵ� DC�� ����
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // _hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC�� ��Ʈ�� ����
	::DeleteObject(prev);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->Init();
	GET_SINGLE(ResourceManager)->Init(hwnd, fs::path(L"E:\\Cpp\\practice_winapi\\GameCoding\\Resources"));
	GET_SINGLE(SoundManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);
}

void Game::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
	srand(unsigned(time(0)));
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);

	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	{ // ���콺 ��ġ ���
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{ // FPS, Delta Time ���
		wstring str = std::format(L"FPS({0}), DT({1})", fps, static_cast<int32>(deltaTime * 1000));
		::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// ���� ���۸�, ��Ʈ ���� ; ���� ����
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY);

	// ���� ���� �� ���� Ŭ����
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);

}