#include "pch.h"
#include "Quest1Trigger.h"
#include "Player.h"
#include "GameRoom.h"
#include "GameSession.h"

Quest1Trigger::Quest1Trigger()
{
	_questID = 1;
}

Quest1Trigger::~Quest1Trigger()
{

}

void Quest1Trigger::Tick()
{
	Super::Tick();

	if (_isPlayerOnTile && _complete)
	{
		PlayerRef player = static_pointer_cast<Player>(GRoom->GetCreatureAt(GetCellPos()));
		if (player)
		{
			int playerID = player->GetObjectID();
			player->room->SetQuestStateProgress(playerID, 1, 1);

			auto questState = player->GetQuestState(1);

			if (questState == Protocol::QUEST_STATE_ACCEPT)
			// 완료 패킷 전송
			{
 				player->room->SetQuestStates(playerID, 1, Protocol::QUEST_STATE_COMPLETED);

				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_QuestComplete(playerID, 1, 1);
				player->session->Send(sendBuffer);
			}
		}
	}
}

bool Quest1Trigger::IsPlayerOnTile()
{
	if (Super::IsPlayerOnTile())
	{
		if (_questID == 1)
		{
			PlayerRef player = static_pointer_cast<Player>(GRoom->GetCreatureAt(GetCellPos()));
			if (player)
			{
				auto questState = player->GetQuestState(1);

				if (questState != Protocol::QUEST_STATE_ACCEPT)
					return false;

				auto state = player->info.state();

				if (state == Protocol::OBJECT_STATE_TYPE_SPIN_READY)
				{
					SetComplete(true);
					return true;
				}
			}
		}
		return false;
	}
}
