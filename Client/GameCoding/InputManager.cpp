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

int InputManager::ProcessSpecialKeys(int key)
{
	switch (key)
	{
	// LShift
	case 16:
		return -1;

	case 188:
		return L',';

	case 190:
		return L'.';

	case 191:
		return L'/';

	case 186:
		return L';';

	case 222:
		return L'\'';

	case 219:
		return L'[';

	case 221:
		return L']';

	case 220:
		return L'\\';

	case 189:
		return L'-';

	case 187:
		return L'=';

	default:
		return key;
	}
	return -1;
}

int InputManager::ProcessSpecialShiftKeys(int key)
{
	switch (key)
	{
	// 특수 문자열
	case 188:
		return L'<';

	case 190:
		return L'>';

	case 191:
		return L'?';

	case 186:
		return L':';

	case 222:
		return L'\"';

	case 219:
		return L'{';

	case 221:
		return L'}';

	case 220:
		return L'|';

	case 189:
		return L'_';

	case 187:
		return L'+';

	// 쉬프트 키를 포함한 일반 문자 입력시
	// 대문자->소문자, 소문자->대문자 변환이 필요
	default:
		return key;
	}
	return -1;
}
