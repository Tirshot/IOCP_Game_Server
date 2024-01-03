#include "pch.h"
#include "Chat.h"
#include "MyPlayer.h"
#include "ChatManager.h"
#include "SceneManager.h"
#include "InputManager.h"

void ChatManager::BeginPlay()
{
	
}

void ChatManager::Tick()
{
	if (_myPlayer)
		return;

	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
}

void ChatManager::AddMessage(const wstring text)
{
	_chat->AddText(text);
}

string ChatManager::WStrToString(wstring wstr)
{
	// wstring to string 변환
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	string str2(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

	return str2;
}

wstring ChatManager::StringToWStr(string str)
{
	// string to wstring 변환
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

	wstring str2(bufferSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize);

	return str2;
}

void ChatManager::ChatInput()
{
	if (_myPlayer == nullptr)
		return;

	// Enter 입력된 상태, 채팅창을 활성화 함
	_chat->SetVisibleChat();

	if (_chatSended)
	{
		_myPlayer->SetState(IDLE);
		_chatSended = false;
		return;
	}

	// 엔터 키가 눌리면 입력 종료
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Enter))
	{
		// str을 패킷으로 전송
		{
			GET_SINGLE(ChatManager)->AddMessage(_str);
		}
		_str.clear();
		_chatSended = true;
		_myPlayer->SetState(IDLE);
	}

	// 키 입력 받기
	for (int i = 0; i <= 255; ++i) // 'A'부터 'Z'까지
	{
		KeyType key = static_cast<KeyType>(i);

		// Shift를 누른 채로 특수문자 키 확인
		if (GET_SINGLE(InputManager)->GetButton(KeyType::RightShift)
			&& GET_SINGLE(InputManager)->GetButtonDown(key)
			|| GET_SINGLE(InputManager)->GetButton(KeyType::LeftShift)
			&& GET_SINGLE(InputManager)->GetButtonDown(key))
		{
			auto ret = GET_SINGLE(InputManager)->ProcessSpecialShiftKeys(i);
			_str.push_back(static_cast<wchar_t>(ret));
		}

		// 특수문자 키 확인
		else if (GET_SINGLE(InputManager)->GetButtonDown(key)) // 키가 눌렸는지 확인
		{
			auto ret = GET_SINGLE(InputManager)->ProcessSpecialKeys(i);

			// 이상한 입력을 받으면 ret가 음수로 나옴
			if (ret < 0)
				return;

			_str.push_back(static_cast<wchar_t>(ret));
		}
	}
}


wstring ChatManager::ChangeTimeFormat(time_t now)
{
	// std::tm 구조체를 사용하여 시:분:초로 변환
	std::tm localTime;
	localtime_s(&localTime, &now);

	// 문자열로 포맷팅
	char buffer[9]; // "HH:MM:SS\0"까지 9자리 필요
	strftime(buffer, sizeof(buffer), "%H:%M:%S", &localTime);

	wchar_t wideBuffer[9];
	mbstowcs_s(nullptr, wideBuffer, buffer, sizeof(buffer));

	// wchar_t 배열을 wstring으로 변환
	wstring wideTime(wideBuffer);

	return wideTime;
}
