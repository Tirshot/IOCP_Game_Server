#include "pch.h"
#include "NetworkManager.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "ChatManager.h"
#include "Chat.h"

void NetworkManager::Init()
{
	// 소켓 생성
	SocketUtils::Init();

	_service = make_shared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); }, // 관찰 대상 등록
		1);

	assert(_service->Start());

	//for (int32 i = 0; i < 5; i++)
	//{
	//	GThreadManager->Launch([=]()
	//		{
	//			while (true)
	//			{
	//				service->GetIocpCore()->Dispatch();
	//			}
	//		});
	//}
}

void NetworkManager::Update()
{
	// Timeout = 0, 바로 빠져나옴
	_service->GetIocpCore()->Dispatch(0);
}

ServerSessionRef NetworkManager::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetworkManager::SendPacket(SendBufferRef sendBuffer)
{
	if (_session)
		_session->Send(sendBuffer);
}
