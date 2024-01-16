#include "pch.h"
#include "MerchantTutorialTrigger.h"
#include "DevScene.h"
#include "MerchantTutorial.h"
#include "ResourceManager.h"
#include "SceneManager.h"

MerchantTutorialTrigger::MerchantTutorialTrigger()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	// 튜토리얼 UI 노출
	MerchantTutorial* tu = new MerchantTutorial();

	_tutorial = tu;
	_tutorial->SetVisible(false);
	_tutorial->SetPos({ 490,180 });
	scene->AddUI(_tutorial);
}

MerchantTutorialTrigger::~MerchantTutorialTrigger()
{
}

void MerchantTutorialTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void MerchantTutorialTrigger::Tick()
{
	Super::Tick();

	if (_tutorial == nullptr)
		return;

	TouchInteract(_tutorial);
}
