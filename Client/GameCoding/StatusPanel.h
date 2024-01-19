#pragma once
#include "Panel.h"
class StatusPanel : public Panel
{
	using Super = Panel;

public:
	StatusPanel();
	virtual ~StatusPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

protected:
	RECT _rect = {};
};

