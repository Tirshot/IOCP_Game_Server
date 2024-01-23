#pragma once
#include "Player.h"
class Texture;
class MyPlayer :  public Player
{
	using Super = Player;
public:
	MyPlayer();
	virtual ~MyPlayer() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

public:
	void Teleport(Vec2Int cellPos);
	int GetSelectedSlot();

	void UsePotion();
	int GetPotionNums() { return info.potion(); }

private:
	void TickInput();
	void TryMove();

	virtual void TickIdle() override;
	virtual void TickMove() override;
	virtual void TickSkill() override;
	virtual void TickHit() override;
	virtual void TickChat() override;
	virtual void TickSpin() override;
	virtual void TickSpinReady() override;
	virtual void TickTeleport() override;

	void SyncToServer();

public:
	map<int, pair<Protocol::QUEST_STATE, int>>& GetQuestStates() { return _questsStates; }
	Protocol::QUEST_STATE& GetQuestState(int questId) { return _questsStates[questId].first; }
	void SetQuestState(int questId, Protocol::QUEST_STATE state, int progress) { _questsStates[questId] = { state, progress }; }

	int GetQuestProgress(int questId);
	void SetQuestProgress(int questId, int progress);
	void AddQuestProgress(int questId) { _questsStates[questId].second += 1; }

private:
	map<int, pair<Protocol::QUEST_STATE, int>> _questsStates;

	bool _keyPressed = false;
	Texture* _plum = nullptr;
	float _sumTimes = 0.f;
	int _selectedSlot = 0;
	int _potionNums = 0;
};

