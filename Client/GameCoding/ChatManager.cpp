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
	// wstring to string ��ȯ
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	string str2(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

	return str2;
}

wstring ChatManager::StringToWStr(string str)
{
	// string to wstring ��ȯ
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

	wstring str2(bufferSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize);

	return str2;
}

void ChatManager::ChatInput()
{
	if (_myPlayer == nullptr)
		return;

	// Enter �Էµ� ����, ä��â�� Ȱ��ȭ ��
	_chat->SetVisibleChat();

	if (_chatSended)
	{
		_myPlayer->SetState(IDLE);
		_chatSended = false;
		return;
	}

	// ���� Ű�� ������ �Է� ����
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Enter))
	{
		// str�� ��Ŷ���� ����
		{
			GET_SINGLE(ChatManager)->AddMessage(_str);
		}
		_str.clear();
		_chatSended = true;
		_myPlayer->SetState(IDLE);
	}

	// Ű �Է� �ޱ�
	for (int i = 0; i <= 255; ++i) // 'A'���� 'Z'����
	{
		KeyType key = static_cast<KeyType>(i);

		// Shift�� ���� ä�� Ư������ Ű Ȯ��
		if (GET_SINGLE(InputManager)->GetButton(KeyType::RightShift)
			&& GET_SINGLE(InputManager)->GetButtonDown(key)
			|| GET_SINGLE(InputManager)->GetButton(KeyType::LeftShift)
			&& GET_SINGLE(InputManager)->GetButtonDown(key))
		{
			auto ret = GET_SINGLE(InputManager)->ProcessSpecialShiftKeys(i);
			_str.push_back(static_cast<wchar_t>(ret));
		}

		// Ư������ Ű Ȯ��
		else if (GET_SINGLE(InputManager)->GetButtonDown(key)) // Ű�� ���ȴ��� Ȯ��
		{
			auto ret = GET_SINGLE(InputManager)->ProcessSpecialKeys(i);

			// �̻��� �Է��� ������ ret�� ������ ����
			if (ret < 0)
				return;

			_str.push_back(static_cast<wchar_t>(ret));
		}
	}
}


wstring ChatManager::ChangeTimeFormat(time_t now)
{
	// std::tm ����ü�� ����Ͽ� ��:��:�ʷ� ��ȯ
	std::tm localTime;
	localtime_s(&localTime, &now);

	// ���ڿ��� ������
	char buffer[9]; // "HH:MM:SS\0"���� 9�ڸ� �ʿ�
	strftime(buffer, sizeof(buffer), "%H:%M:%S", &localTime);

	wchar_t wideBuffer[9];
	mbstowcs_s(nullptr, wideBuffer, buffer, sizeof(buffer));

	// wchar_t �迭�� wstring���� ��ȯ
	wstring wideTime(wideBuffer);

	return wideTime;
}
