#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
#include <atomic>
#include <mutex>
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "Listener.h"
#include "Service.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"

int main()
{
	SocketUtils::Init();
	GRoom->Init();

	// 연산의 프레임 제한을 위함. 싱글 스레드로 가정
	int16 frameCounter = 0;

	ServerServiceRef service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[](){ return make_shared<GameSession>(); }, // TODO : SessionManager 등
		100);

	assert(service->Start());

	//for (int32 i = 0; i < 5; i++)
	//{
	//	GThreadManager->Launch([=]()
	//		{
				// 싱글 스레드로 만듬
				while (true)
				{
					service->GetIocpCore()->Dispatch(0);
					// 15 프레임마다 업데이트
					if (frameCounter % 15 == 0)
					{
						GRoom->Update();
						frameCounter = 0;
					}
					frameCounter++;
				}
				/*
			});
	}*/


	GThreadManager->Join();
	
	// Windows Socket 종료
	SocketUtils::Clear();	
}