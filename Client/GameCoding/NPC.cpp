#include "pch.h"
#include "NPC.h"

NPC::NPC()
{

}

NPC::~NPC()
{

}

void NPC::BeginPlay()
{
	Super::BeginPlay();
}

void NPC::Tick()
{
	Super::Tick();
}

void NPC::Render(HDC hdc)
{
	Super::Render(hdc);
}

void NPC::TickIdle()
{

}

void NPC::TickMove()
{

}

void NPC::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove);
		break;
	}
}
