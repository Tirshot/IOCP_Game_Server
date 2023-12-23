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

private:
	Chat* _chat;
};

