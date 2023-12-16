#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	// 환영
	Send(ServerPacketHandler::Make_S_EnterGame());

	// 게임 입장
	GRoom->EnterRoom(GetSessionRef());

}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	// 게임 퇴장
	GRoom->LeaveRoom(GetSessionRef());
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	ServerPacketHandler::HandlePacket(GetSessionRef(), buffer, len);
}

void GameSession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}