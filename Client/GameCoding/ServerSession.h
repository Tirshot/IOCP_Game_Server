#pragma once
#include "ClientPacketHandler.h"
class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{

	}

	virtual void OnConnected() override
	{

	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		ClientPacketHandler::HandlePacket(buffer, len);
	}

	virtual void OnSend(int32 len) override
	{

	}

	virtual void OnDisconnected() override
	{

	}
};