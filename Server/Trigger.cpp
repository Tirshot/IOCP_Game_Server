#include "pch.h"
#include "Trigger.h"
#include "Player.h"
#include "GameRoom.h"
#include "Quest1Trigger.h"

Trigger::Trigger()
{
	_prev = GetTickCount64();
}

Trigger::~Trigger()
{

}

void Trigger::Tick()
{
	if (_questID == 0)
		return;

	_now = GetTickCount64();

	if (_now - _prev >= 500)
	{
		if (IsPlayerOnTile())
		{
			PlayerRef player = static_pointer_cast<Player>(GRoom->GetCreatureAt(GetCellPos()));

			if (player)
			{
				_isPlayerOnTile = true;
			}
		}
		else
		{
			_isPlayerOnTile = false;
		}

		_prev = _now;
	}
}

bool Trigger::IsPlayerOnTile()
{
	if (_questID == 0)
		return false;

	PlayerRef player = static_pointer_cast<Player>(GRoom->GetCreatureAt(GetCellPos()));

	if (player)
		return true;

	return false;
}