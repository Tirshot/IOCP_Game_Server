#pragma once

using ServerSessionRef = shared_ptr<class ServerSession>;

class NetworkManager
{
	DECLARE_SINGLE(NetworkManager);

public:
	void Init();
	void Update();

	ServerSessionRef CreateSession();
	void RemoveSession();
	void SendPacket(SendBufferRef sendBuffer);
	bool isSessionAlive();

private:
	ClientServiceRef _service;
	ServerSessionRef _session;
};

