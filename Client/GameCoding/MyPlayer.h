#pragma once
#include "Player.h"
class MyPlayer :  public Player
{
	using Super = Player;
public:
	MyPlayer();
	virtual ~MyPlayer() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;
	void Handle_S_Fire(const Protocol::ObjectInfo& info, uint64 id);

private:
	void TickInput();
	void TryMove();

	virtual void TickIdle() override;
	virtual void TickMove() override;
	virtual void TickSkill() override;
	virtual void TickHit() override;

	void SyncToServer();

	bool _keyPressed = false;
};

