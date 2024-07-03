#include "pch.h"
#include "MerchantTutorialTrigger.h"
#include "DevScene.h"
#include "MerchantTutorial.h"
#include "ResourceManager.h"
#include "SceneManager.h"

MerchantTutorialTrigger::MerchantTutorialTrigger()
{
}

MerchantTutorialTrigger::~MerchantTutorialTrigger()
{
	_tutorial = nullptr;
}

void MerchantTutorialTrigger::BeginPlay()
{
	Super::BeginPlay();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	// 튜토리얼 UI 노출
	auto tu = make_shared<MerchantTutorial>();

	if (scene == nullptr)
		return;

	_tutorial = tu;
	_tutorial->SetVisible(false);
	_tutorial->SetPos({ 490,180 });
	scene->AddUI(_tutorial);
}

void MerchantTutorialTrigger::Tick()
{
	Super::Tick();

	if (_tutorial == nullptr)
		return;

	TouchInteract(_tutorial);
}
