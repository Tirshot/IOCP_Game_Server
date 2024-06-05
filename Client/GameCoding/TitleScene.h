#pragma once
#include "Scene.h"

class Actor;
class UI;
class GameObject;
class Player;
class Sprite;

class TitleScene :  public Scene
{
	using Super = Scene;

public:
	TitleScene();
	virtual ~TitleScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

public:
	void LoadPlayer();
	void LoadUI();
	void LoadSound();
	

private:
	void BackgroundFadeInAndOut();
	void PressAnyToStart();
	void OnClickExitButton();
	void OnClickOptionButton();

private:
	Sprite* _background = nullptr;
	Sprite* _title = nullptr;
	Sprite* _black = nullptr;
	Sprite* _pressToStart = nullptr;
	float _sumTime = 0.f;
	float _randX = 0.f;
	float _randY = 0.f;
	float _dir1 = 1.f;
	float _dir2 = 0.f;
	int _multiplier = 50;
	int _alpha = 255;
	bool _isFadedIn = false;
	bool _isPressedAnyKey = false;
};

