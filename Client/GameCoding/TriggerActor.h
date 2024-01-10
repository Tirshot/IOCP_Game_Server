#pragma once
#include "GameObject.h"
class TriggerActor : public GameObject
{
	using Super = Actor;
public:
	TriggerActor();
	virtual ~TriggerActor();

	virtual void BeginPlay();
	virtual void Tick();
};

