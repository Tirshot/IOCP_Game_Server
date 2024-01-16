#pragma once
#include "TriggerActor.h"
class MerchantTutorial;
class Flipbook;
class MerchantTutorialTrigger : public TriggerActor
{
	using Super = TriggerActor;
public:
	MerchantTutorialTrigger();
	virtual ~MerchantTutorialTrigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

protected:
	MerchantTutorial* _tutorial = nullptr;
};

