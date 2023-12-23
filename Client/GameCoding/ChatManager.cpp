#include "pch.h"
#include "ChatManager.h"
#include "Chat.h"

void ChatManager::BeginPlay()
{
	
}

void ChatManager::Tick()
{

}

void ChatManager::AddMessage(const wstring text)
{
	GetChat()->AddText(text);
}
