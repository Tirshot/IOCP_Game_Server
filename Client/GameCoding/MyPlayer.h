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
	int GetPotionMaxCount() { return _potionMaxCount; }
	int GetPotionEffect() { return _potionEffect; }

	void SetPotionMaxCount(int count) { _potionMaxCount = count; }
	void SetPotionEffect(int effect) { _potionEffect = effect; }

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

public:
	void SyncToServer();

public:
	map<int, pair<Protocol::QUEST_STATE, int>>& GetQuestStates() { return _questsStates; }
	Protocol::QUEST_STATE& GetQuestState(int questId) { return _questsStates[questId].first; }
	void SetQuestState(int questId, Protocol::QUEST_STATE state, int progress) { _questsStates[questId] = { state, progress }; }
	void SetQuestState(int questId, Protocol::QUEST_STATE state) { _questsStates[questId].first = state; }
	int GetQuestProgress(int questId);
	map<int, pair<Protocol::QUEST_STATE, int>> GetActiveQuests();
	void SetQuestProgress(int questId, int progress);
	void AddQuestProgress(int questId) { _questsStates[questId].second += 1; }

private:
	map<int, pair<Protocol::QUEST_STATE, int>> _questsStates;

	bool _keyPressed = false;
	shared_ptr<Texture> _plum = nullptr;
	shared_ptr<class MiniMap> _miniMap = nullptr;
	float _sumTimes = 0.f;
	int _selectedSlot = 0;
	int _potionNums = 0;
	int _potionMaxCount = 5;
	int _potionEffect = 1;	// 포션 효과 배율
};

