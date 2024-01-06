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

	void SyncToServer();

	bool _keyPressed = false;
	bool _isSpinning = false;
};

