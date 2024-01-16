#pragma once
#include "TriggerActor.h"
class MerchantUI;
class Flipbook;
class MerchantTrigger : public TriggerActor
{
	using Super = TriggerActor;
public:
	MerchantTrigger();
	virtual ~MerchantTrigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;

protected:
	MerchantUI* _merchantUI = nullptr;
};

