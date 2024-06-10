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
	void SetUI(shared_ptr<Panel> panel) { _ui = panel; }

protected:
	shared_ptr<Panel> _ui = nullptr;
};

