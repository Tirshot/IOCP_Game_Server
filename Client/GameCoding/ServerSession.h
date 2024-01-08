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
		GET_SINGLE(ChatManager)->AddMessage(L"������ ����Ǿ����ϴ�. \nEnter�� ���� ä���� �����غ�����.");
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
		GET_SINGLE(ChatManager)->AddMessage(L"�������� ������ ���������ϴ�.");
		GET_SINGLE(SceneManager)->SetPause(true);
	}
};
