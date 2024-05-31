#include "pch.h"
#include "Chat.h"
#include "SendBuffer.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "Player.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

extern ChatRef GChat = make_shared<Chat>();

Chat::Chat()
{

}

Chat::~Chat()
{

}

void Chat::BeginPlay()
{
	AddText(L"System : ä���� Ȱ��ȭ��.");
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
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendMessage(objectId, now, str2);
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
		//SendBufferRef sendBuffer = ServerPacketHandler::Make_S_SendMessage();
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

void Chat::AddChatToLog(uint64 objectId, time_t time, wstring str)
{
	_log[objectId].push_back(pair<time_t, wstring>(time, str));
}

string Chat::WStrToString(wstring wstr)
{
	// wstring to string ��ȯ
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	string str2(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str2[0], bufferSize, nullptr, nullptr);

	return str2;
}

wstring Chat::StringToWStr(string str)
{
	// string to wstring ��ȯ
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

	wstring str2(bufferSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &str2[0], bufferSize);

	return str2;
}

void Chat::SaveLogFile()
{
	fs::path _logPath = fs::path(L"E:\\Cpp\\IOCP\\Server\\Server\\Log");
	// C++ ��Ÿ��
	{
		wofstream ofs;

		wstring logFilename = L"Log_" + GetDateForLogName() + L".txt";
		fs::path logFilePath = _logPath / logFilename;

		ofs.open(logFilePath, ios_base::out | ios_base::app);
		if (ofs.is_open())
		{
			// _log ��ȸ
			for (auto& log : _log)
			{
				/*int objectId = log.first;*/
				list<pair<time_t, wstring>>& logList = log.second;

				/*ofs << L"Object ID: " << objectId << endl;*/

				// log ��ȸ
				for (auto& texts : logList)
				{
					time_t time = texts.first;
					wstring wstr = texts.second;

					// �ð� ǥ��
					wstring timeStr = SetTimeFormat(time);

					ofs << timeStr << wstr << endl;
				}
				ofs << endl;
			}
			ofs.close();
		}
		else
		{
			cout << "�α����� ���� ����" << endl;
		}
	}
	_log.clear();
}

wstring Chat::GetDateForLogName()
{
	auto now = std::time(nullptr);
	std::tm nowTm;
	localtime_s(&nowTm, &now);

	std::wstringstream wss;
	wss << std::put_time(&nowTm, L"%Y%m%d");
	return wss.str();
}

wstring Chat::SetTimeFormat(time_t time)
{
	std::tm timeInfo;
	localtime_s(&timeInfo, &time);

	std::wstringstream wss;
	wss << std::put_time(&timeInfo, L"[%H:%M:%S] ");
	return wss.str();
}