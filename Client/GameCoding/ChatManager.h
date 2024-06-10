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

	shared_ptr<Chat> GetChat() { return _chat; }
	void SetChat(shared_ptr<Chat> chat) { _chat = chat; }

	shared_ptr<ChatInput> GetChatInput() { return _chatInput; }
	void SetChatInput(shared_ptr<ChatInput> chatInput) { _chatInput = chatInput; }

	void AddMessage(const wstring text);

	string WStrToString(wstring wstr);
	wstring StringToWStr(string str);

	wstring ChangeTimeFormat(time_t now);

	void SetVisibleChat();
	void SetInvisibleChat();
	void SetVisibleChatInput(bool visible);

	void SendMessageToServer(const wstring text, bool broadcast = true);

private:
	shared_ptr<Chat> _chat = nullptr;
	shared_ptr<ChatInput> _chatInput = nullptr;
	wstring _str = L"";
};

