#pragma once
class MyPlayer;
class Chat;
class ChatInput;
class ChatManager
{
	DECLARE_SINGLE(ChatManager);

public:
	void BeginPlay();
	void Tick();

	Chat* GetChat() { return _chat; }
	void SetChat(Chat* chat) { _chat = chat; }

	ChatInput* GetChatInput() { return _chatInput; }
	void SetChatInput(ChatInput* chatInput) { _chatInput = chatInput; }

	void AddMessage(const wstring text);

	string WStrToString(wstring wstr);
	wstring StringToWStr(string str);

	wstring ChangeTimeFormat(time_t now);

	void SetVisibleChat();
	void SetInvisibleChat();
	void SetVisibleChatInput(bool visible);

	void SendMessageToServer(const wstring text);

private:
	Chat* _chat = nullptr;
	ChatInput* _chatInput = nullptr;
	wstring _str = L"";
};

