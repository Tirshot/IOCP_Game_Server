#pragma once
#include "TriggerActor.h"
class Panel;
class Trigger : public TriggerActor
{
	using Super = TriggerActor;
public:
	Trigger();
	virtual ~Trigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

public:
	void SetUI(Panel* panel) { _ui = panel; }

protected:
	Panel* _ui = nullptr;
};

