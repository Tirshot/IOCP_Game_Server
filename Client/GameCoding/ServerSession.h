#pragma once
#include "ClientPacketHandler.h"
#include "ChatManager.h"
#include "SceneManager.h"

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{

	}

	virtual void OnConnected() override
	{
		GET_SINGLE(ChatManager)->AddMessage(L"서버와 연결되었습니다.");
		GET_SINGLE(SceneManager)->SetPause(false);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		ClientPacketHandler::HandlePacket(static_pointer_cast<ServerSession>(shared_from_this()) , buffer, len);
	}

	virtual void OnSend(int32 len) override
	{

	}

	virtual void OnDisconnected() override
	{
		GET_SINGLE(ChatManager)->AddMessage(L"서버와의 연결이 끊어졌습니다.\n5초 후 서버에 연결을 시도합니다.");
		GET_SINGLE(SceneManager)->SetPause(false);
	}
};
