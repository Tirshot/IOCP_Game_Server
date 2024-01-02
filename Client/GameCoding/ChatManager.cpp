#include "pch.h"
#include "ChatManager.h"
#include "Chat.h"

void ChatManager::BeginPlay()
{
	
}

void ChatManager::Tick()
{

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
