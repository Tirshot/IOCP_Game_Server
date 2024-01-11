#include "pch.h"
#include "Item.h"
#include "Sprite.h"

Item::Item()
{
	SetLayer(LAYER_BACKGROUND);
	SetState(IDLE);
}

Item::~Item()
{
}

void Item::BeginPlay()
{
	Super::BeginPlay();
}

void Item::Tick()
{
	Super::Tick();
}

void Item::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Item::TickIdle()
{

}