#pragma once
class Chat;
class ChatManager
{
	DECLARE_SINGLE(ChatManager);

public:
	void BeginPlay();
	void Tick();

	Chat* GetChat() { return _chat; }
	void SetChat(Chat* chat) { _chat = chat; }

	void AddMessage(const wstring text);

	string WStrToString(wstring wstr);
	wstring StringToWStr(string str);

	wstring ChangeTimeFormat(time_t now);

private:
	Chat* _chat;
};

