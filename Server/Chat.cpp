#include "pch.h"
#include "Chat.h"

extern ChatRef GChat = make_shared<Chat>();

Chat::Chat()
{

}

Chat::~Chat()
{

}

void Chat::BeginPlay()
{
	AddText(L"채팅이 활성화됨.");
}

void Chat::Tick()
{
	if (_texts.empty())
		return;

	PrintText();
}

void Chat::AddText(int objectId, const wstring str)
{
	auto& textList = _texts[objectId];
	textList.push_back(pair<time_t, wstring>(time(0), str));

	// 로그에 복사
	
	// text 내용을 전달하는 패킷 작성
	{

	}
}

void Chat::AddText(const wstring str)
{
	// 시스템 메세지
	auto& textList = _texts[0];
	textList.push_back(pair<time_t, wstring>(time(0), str));

	// 로그에 복사

	// text 내용을 전달하는 패킷 작성
	{

	}
}

void Chat::PrintText()
{
	// 콘솔에 텍스트 출력
	for (auto it = _texts.begin(); it != _texts.end();)
	{
		int objectId = it->first;
		list<pair<time_t, wstring>>& pairs = it->second;

		for (const auto& text : pairs)
		{
			time_t time = text.first;
			const wstring str = text.second;

			if (objectId == 0)
				wcout<< format(L"[{0}] System : {1}", time, str) << endl;
			else
				wcout << format(L"[{0}] Player {1} : {2}", time, objectId, str) << endl;
		}
		// 출력된 텍스트를 삭제
		it = _texts.erase(it);
	}
}
