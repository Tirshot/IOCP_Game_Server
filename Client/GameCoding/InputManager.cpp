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
	// Ư�� ���ڿ�
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

	// ����Ʈ Ű�� ������ �Ϲ� ���� �Է½�
	// �빮��->�ҹ���, �ҹ���->�빮�� ��ȯ�� �ʿ�
	default:
		return key;
	}
	return -1;
}
