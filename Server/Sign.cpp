#include "pch.h"
#include "Sign.h"

Sign::Sign()
{

}

Sign::~Sign()
{

}

void Sign::BeginPlay()
{
}

void Sign::Tick()
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

void Sign::TickIdle()
{

}

void Sign::TickMove()
{

}