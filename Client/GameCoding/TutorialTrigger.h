#pragma once
#include "TriggerActor.h"
class Tutorial;
class Flipbook;
class TutorialTrigger : public TriggerActor
{
	using Super = TriggerActor;
public:
	TutorialTrigger();
	virtual ~TutorialTrigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

protected:
	shared_ptr<Tutorial> _tutorial = nullptr;
};

