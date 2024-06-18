#pragma once
#include "Panel.h"
class Sprite;
class MiniMap : public Panel
{
	using Super = Panel;

public:
	MiniMap();
	virtual ~MiniMap();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

private:
	shared_ptr<Sprite> _map = nullptr;
	shared_ptr<Sprite> _background = nullptr;
	float _scale = 2;
};

