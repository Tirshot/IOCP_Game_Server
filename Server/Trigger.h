#pragma once
#include "GameObject.h"
class Trigger :  public GameObject
{
	// Ʈ���Ÿ� ����ϱ� ���� SetPos, SetQuestID �ʼ�
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

	// �ʴ� ���� ȸ�� ���
	float _now = 0;
	float _prev = 0;

	bool _complete = false;
	bool _isPlayerOnTile = false;
};

