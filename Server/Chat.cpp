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
	AddText(L"ä���� Ȱ��ȭ��.");
}

void Chat::Tick()
{
	if (_texts.empty())
		return;

	PrintText();
}

void Chat::AddText(int objectId, const wstring str)
{
	// �ؽ�Ʈ�� �޴� ����� �÷��̾ �ƴ� ��� ����
	if (GRoom->FindObject(objectId)->GetType() != Protocol::OBJECT_TYPE_PLAYER)
		return;

	auto& textList = _texts[objectId];
	uint64 now = time(0);
	textList.push_back(pair<time_t, wstring>(now, str));

	// �α׿� ����
	{
		_log[objectId].push_back(pair<time_t, wstring>(now, str));
	}

	// text ������ �����ϴ� ��Ŷ �ۼ�
	{
		// wstring to string ��ȯ
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);

		string str2(bufferSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

		// �ش� �������Ը� ���̴� ���� ����
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendChat(objectId, now, str2);
		PlayerRef player = dynamic_pointer_cast<Player>(GRoom->FindObject(objectId));
		player->session->Send(sendBuffer);
	}
}

void Chat::AddText(const wstring str)
{
	// �ý��� �޼���
	auto& textList = _texts[0];
	uint64 now = time(0);
	textList.push_back(pair<time_t, wstring>(now, str));

	// �α׿� ����
	{
		_log[0].push_back(pair<time_t, wstring>(now, str));
	}

	// text ������ �����ϴ� ��Ŷ �ۼ�
	{
		// ���� �˸� ����
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
	// �ֿܼ� �ؽ�Ʈ ���
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
		// ��µ� �ؽ�Ʈ�� ����
		it = _texts.erase(it);
	}
}
