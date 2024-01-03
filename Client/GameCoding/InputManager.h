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
    RightShift = VK_RSHIFT,
    LeftShift = VK_LSHIFT,

    // 알파벳 대문자 A부터 Z까지
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    F = 'F',
    G = 'G',
    H = 'H',
    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',
    M = 'M',
    N = 'N',
    O = 'O',
    P = 'P',
    Q = 'Q',
    R = 'R',
    S = 'S',
    T = 'T',
    U = 'U',
    V = 'V',
    W = 'W',
    X = 'X',
    Y = 'Y',
    Z = 'Z',

    // 특수문자
    KEY_Comma = ',',
    KEY_Period = '.',
    KEY_Slash = '/',
    KEY_Semicolon = ';',
    KEY_Apostrophe = '\'',
    KEY_LeftBracket = '[',
    KEY_RightBracket = ']',
    KEY_BackSlash = '\\',
    KEY_Minus = '-',
    KEY_Equals = '=',
    KEY_GraveAccent = '`',
    KEY_Tilde = '~',
    KEY_Exclamation = '!',
    KEY_At = '@',
    KEY_Sharp = '#',
    KEY_Dollar = '$',
    KEY_Percent = '%',
    KEY_Caret = '^',
    KEY_And = '&',
    KEY_Star = '*',
    KEY_LeftParenthesis = '(',
    KEY_RightParenthesis = ')',
    KEY_UnderBar = '_',
    KEY_Plus = '+',

    Backspace = VK_BACK,

    // 숫자 0부터 9까지
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
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

	// 누르고 있을 때
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	// 맨 처음에 눌렀을 때
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }
    bool GetButtonDown(int key) { return GetState(key) == KeyState::Down; }
	// 눌렀다가 땠을 때
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	POINT GetMousePos() { return _mousePos; }
	KeyState SetButtonUp(KeyType key) { return _states[static_cast<uint8>(key)] = KeyState::Up; }

    int ProcessSpecialKeys(int key);
    int ProcessSpecialShiftKeys(int key);

	bool ChatSend() { _send = true; }

private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }
	KeyState GetState(int key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;
	vector<KeyState> _states;
	POINT _mousePos = {};
	bool _send = false;
};

