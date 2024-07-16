#include "pch.h"
#include "NPC.h"

NPC::NPC()
{
	info.set_objecttype(Protocol::OBJECT_TYPE_NPC);

}

NPC::~NPC()
{

}

void NPC::BeginPlay()
{
}

void NPC::Tick()
{
	switch (info.state())
	{
	case IDLE:
		TickIdle();
		break;

	case MOVE:
		TickMove();
		break;
	}
}

void NPC::TickIdle()
{

}

void NPC::TickMove()
{

}
