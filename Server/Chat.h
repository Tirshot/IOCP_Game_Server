#pragma once

class Chat
{
public:
	Chat();
	virtual ~Chat();

	virtual void BeginPlay();
	virtual void Tick();

	// 텍스트 관련
	void AddText(int objectId, const wstring str);
	void AddText(const wstring str);

	void SendToClient(int objectId);

	void PrintText();
	list<pair<time_t, wstring>>& GetTextList(int objectId){ return _texts[objectId]; }

private:
	float _now = 0;
	map<int, list<pair<time_t, wstring>>> _texts;
	map<int, list<pair<time_t, wstring>>> _log;
};

extern ChatRef GChat;