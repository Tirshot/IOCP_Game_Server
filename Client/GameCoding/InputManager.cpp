#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 256개로 된 배열에서 내가 누른 키가 있는가?
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 막 눌렀다면 PRESS
			if (state == KeyState::Press || state == KeyState::Down)
				// 키를 지금 방금 눌렀다면
				state = KeyState::Press;
			else
				// 키를 누르고 있다면
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 누르고 있었다면  Up
			if (state == KeyState::Press || state == KeyState::Down)
				// 눌렀다가 땠다면
				state = KeyState::Up;
			else
				// 아예 안누른 경우
				state = KeyState::None;
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);
}

bool InputManager::IsMouseOutRect(RECT& rect)
{
	POINT mousePos = GetMousePos();

	if (mousePos.x < rect.left || mousePos.x > rect.right || mousePos.y < rect.top || mousePos.y > rect.bottom)
		return true;

	return false;
}
