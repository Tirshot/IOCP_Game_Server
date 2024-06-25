#pragma once
#include "GameObject.h"
class Player;
class Item : public GameObject
{
	using Super = GameObject;

public:
	Item();
	virtual ~Item() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

	void SetOwner(uint64 ownerId) { _ownerId = ownerId; }
	uint64 GetOwner() { return _ownerId; }

	bool IsTouched(uint64 playerId);
	Protocol::ItemInfo GetItemInfo() { return itemInfo; }

protected:
	virtual void TickIdle();

public:
	bool IsGet() { return _get; }

protected:
	// 삭제를 위한 습득 여부
	bool _get = false;
	uint64 _ownerId = 0;

	// 시간 경과 후 자동 제거
	uint64 _tickStart = 0;
	uint64 _tickNow = 0;

public:
	Protocol::ItemInfo itemInfo;
};

