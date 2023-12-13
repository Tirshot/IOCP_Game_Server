#pragma once
#include "Scene.h"
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

};

