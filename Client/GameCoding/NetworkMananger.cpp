#include "pch.h"
#include "NetworkMananger.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"

void NetworkMananger::Init()
{
	// ���� ����
	SocketUtils::Init();

	_service = make_shared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); }, // TODO : SessionManager ��
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

void NetworkMananger::Update()
{
	// Timeout = 0, �ٷ� ��������
	_service->GetIocpCore()->Dispatch(0);
}

ServerSessionRef NetworkMananger::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetworkMananger::SendPacket(SendBufferRef sendBuffer)
{
	if (_session)
		_session->Send(sendBuffer);
}
