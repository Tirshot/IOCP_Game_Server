#include "pch.h"
#include "Chat.h"
#include "SendBuffer.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"

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
	// 텍스트를 받는 대상이 플레이어가 아닐 경우 리턴
	if (GRoom->FindObject(objectId)->GetType() != Protocol::OBJECT_TYPE_PLAYER)
		return;

	auto& textList = _texts[objectId];
	uint64 now = time(0);
	textList.push_back(pair<time_t, wstring>(now, str));

	// 로그에 복사
	{
		_log[objectId].push_back(pair<time_t, wstring>(now, str));
	}

	// text 내용을 전달하는 패킷 작성
	{
		// wstring to string 변환
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);

		string str2(bufferSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

		// 해당 유저에게만 보이는 내용 전송
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendChat(objectId, now, str2);
		PlayerRef player = dynamic_pointer_cast<Player>(GRoom->FindObject(objectId));
		player->session->Send(sendBuffer);
	}
}

void Chat::AddText(const wstring str)
{
	// 시스템 메세지
	auto& textList = _texts[0];
	uint64 now = time(0);
	textList.push_back(pair<time_t, wstring>(now, str));

	// 로그에 복사
	{
		_log[0].push_back(pair<time_t, wstring>(now, str));
	}

	// text 내용을 전달하는 패킷 작성
	{
		// 서버 알림 전송
		//SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendChat();
		//GRoom->Broadcast(sendBuffer);
	}
}

void Chat::SendToClient(int objectId)
{
	list<pair<time_t, wstring>>& sendTexts = _texts[objectId];


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
