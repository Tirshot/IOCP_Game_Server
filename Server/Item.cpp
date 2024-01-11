#include "pch.h"
#include "Item.h"

Item::Item()
{
	info.set_dir(Protocol::DIR_TYPE_DOWN);
}

Item::~Item()
{
}

void Item::BeginPlay()
{
	
}

void Item::Tick()
{
	if (info.state() == IDLE)
		return;

	SetState(IDLE);
}

void Item::TickIdle()
{
}