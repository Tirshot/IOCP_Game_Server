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
		// 256���� �� �迭���� ���� ���� Ű�� �ִ°�?
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// ���� �����ӿ� Ű�� �� �����ٸ� PRESS
			if (state == KeyState::Press || state == KeyState::Down)
				// Ű�� ���� ��� �����ٸ�
				state = KeyState::Press;
			else
				// Ű�� ������ �ִٸ�
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			// ���� �����ӿ� Ű�� ������ �־��ٸ�  Up
			if (state == KeyState::Press || state == KeyState::Down)
				// �����ٰ� ���ٸ�
				state = KeyState::Up;
			else
				// �ƿ� �ȴ��� ���
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
