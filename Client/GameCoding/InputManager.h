#pragma once

enum class KeyType
{
    LeftMouse = VK_LBUTTON,
    RightMouse = VK_RBUTTON,

    Up = VK_UP,
    Down = VK_DOWN,
    Left = VK_LEFT,
    Right = VK_RIGHT,

    SpaceBar = VK_SPACE,
    Enter = VK_RETURN,
    LeftShift = VK_LSHIFT,
    RightShift = VK_RSHIFT,
    Backspace = VK_BACK,

    W = 'W',
    A = 'A',
    S = 'S',
    D = 'D',
    R = 'R',

    // ���� 0���� 9����
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9'
};


enum class KeyState
{
	None,
	Press,
	Down,
	Up,

	End
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<int32>(KeyState::End),
};

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	void Init(HWND hwnd);
	void Update();

	// ������ ���� ��
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	// �� ó���� ������ ��
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }
	// �����ٰ� ���� ��
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	POINT GetMousePos() { return _mousePos; }
	KeyState SetButtonUp(KeyType key) { return _states[static_cast<uint8>(key)] = KeyState::Up; }

private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }
	KeyState GetState(int key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;
	vector<KeyState> _states;
	POINT _mousePos = {};
};

