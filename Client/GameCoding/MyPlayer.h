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
	void AddPotion(int num);
	int GetPotionNums() { return _potionNums; }

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

	bool _keyPressed = false;
	Texture* _plum = nullptr;
	float _sumTimes = 0.f;
	int _selectedSlot = 0;
	int _potionNums = 0;
};

