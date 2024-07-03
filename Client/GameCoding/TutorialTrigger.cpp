#include "pch.h"
#include "TutorialTrigger.h"
#include "DevScene.h"
#include "Tutorial.h"
#include "ResourceManager.h"
#include "SceneManager.h"


TutorialTrigger::TutorialTrigger()
{
}

TutorialTrigger::~TutorialTrigger()
{
	_tutorial = nullptr;
}

void TutorialTrigger::BeginPlay()
{
	Super::BeginPlay();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return;

	// 튜토리얼 UI 노출
	auto tu = make_shared<Tutorial>();

	_tutorial = tu;
	_tutorial->SetVisible(false);
	_tutorial->SetPos({ 300,50 });
	scene->AddUI(_tutorial);
}

void TutorialTrigger::Tick()
{
	Super::Tick();

	if (_tutorial == nullptr)
		return;

	TouchInteract(_tutorial);
}