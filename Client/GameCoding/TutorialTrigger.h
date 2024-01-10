#pragma once
#include "TriggerActor.h"
class Tutorial;
class TutorialTrigger : public TriggerActor
{
	using Super = TriggerActor;
public:
	TutorialTrigger();
	virtual ~TutorialTrigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

private:
	Tutorial* _tutorial = nullptr;
	bool _visiblity = false;
	float _sumTime = 0.f;
};

