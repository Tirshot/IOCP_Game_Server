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
	auto& textList = _texts[objectId];
	textList.push_back(pair<time_t, wstring>(time(0), str));

	// �α׿� ����
	
	// text ������ �����ϴ� ��Ŷ �ۼ�
	{

	}
}

void Chat::AddText(const wstring str)
{
	// �ý��� �޼���
	auto& textList = _texts[0];
	textList.push_back(pair<time_t, wstring>(time(0), str));

	// �α׿� ����

	// text ������ �����ϴ� ��Ŷ �ۼ�
	{

	}
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
