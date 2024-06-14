#pragma once
#include "Panel.h"
class Sprite;
class GameOver : public Panel
{
	using Super = Panel;

public:
	GameOver();
	virtual ~GameOver();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void FadeIn();
	void HideAllUIs();

	void OnClickReviveButton();
	void OnClickExitButton();

private:
	RECT _rect = {};
	shared_ptr<Sprite> _background = nullptr;
	shared_ptr<Sprite> _gameOver = nullptr;
	int _alpha = 0;
	float _sumTime = 0.f;
	bool _invisibleUIs = false;
};

