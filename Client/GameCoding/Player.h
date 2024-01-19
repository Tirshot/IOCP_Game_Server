#pragma once
#include "Creature.h"

class Flipbook;

class Arrow;
class Player :  public Creature
{
	using Super = Creature;
public:
	Player();
	virtual ~Player() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle() override;
	virtual void TickMove() override;
	virtual void TickSkill() override;
	virtual void TickSpin() override;
	virtual void TickTeleport() override;
	virtual void UpdateAnimation() override;

public:
	virtual void Handle_S_Fire(const Protocol::ObjectInfo& info, uint64 id);
	void SyncToServer();

	map<int, Protocol::QUEST_STATE>& GetQuestStates() { return _questsStates; }
	Protocol::QUEST_STATE& GetQuestState(int questId) { return _questsStates[questId]; }
	void SetQuestState(int questId, Protocol::QUEST_STATE state) { _questsStates[questId] = state; }

	int GetQuestProgress(int questId);
	void SetQuestProgress(int questId, int progress);

private:
	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};
	Flipbook* _flipbookAttack[4] = {};
	Flipbook* _flipbookSpin[4] = {};
	Flipbook* _flipbookSpinReady[4] = {};
	Flipbook* _flipbookBow[4] = {};
	Flipbook* _flipbookStaff[4] = {};

	bool _keyPressed = false;

protected:
	uint64 _now = 0;
	uint64 _prev = 0;
	map<int, Protocol::QUEST_STATE> _questsStates; /* 퀘스트 ID 별 상태*/
	map<int, int> _questsProgress; /* 퀘스트 id 별 진행도*/
};

