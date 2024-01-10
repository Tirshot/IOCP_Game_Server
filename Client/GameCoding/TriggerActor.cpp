#include "pch.h"
#include "TriggerActor.h"

TriggerActor::TriggerActor()
{
	SetLayer(LAYER_EFFECT);
}

TriggerActor::~TriggerActor()
{

}

void TriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void TriggerActor::Tick()
{
	Super::Tick();
}
