#pragma once
#include "GameObject.h"
class Trigger :  public GameObject
{
	// 트리거를 사용하기 전에 SetPos, SetQuestID 필수
public:
	Trigger();
	virtual ~Trigger();

	virtual void Tick() override;

public:
	virtual bool IsPlayerOnTile();
	void SetQuestID(int questId) { _questID = questId; }
	int GetQuestID() { return _questID; }

	void SetComplete(bool ret) { _complete = ret; }
	bool GetComplete() { return _complete; }

protected:
	int _questID = 0;

	// 초당 마나 회복 계산
	float _now = 0;
	float _prev = 0;

	bool _complete = false;
	bool _isPlayerOnTile = false;
};

