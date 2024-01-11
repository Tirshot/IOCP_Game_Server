#pragma once
#include "GameObject.h"
class Item : public GameObject
{
	using Super = GameObject;
public:
	Item();
	virtual ~Item() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

protected:
	virtual void TickIdle();

public:
	bool IsHit() { return _hit; }

protected:
	// 삭제를 위한 적중 여부
	bool _hit = false;
};

