#pragma once
#include "Panel.h"

class ChatInput : public Panel
{
	using Super = Panel;

public:
	ChatInput();
	virtual ~ChatInput();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

private:


};

