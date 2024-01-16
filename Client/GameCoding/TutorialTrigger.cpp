#include "pch.h"
#include "TutorialTrigger.h"
#include "DevScene.h"
#include "Tutorial.h"
#include "ResourceManager.h"
#include "SceneManager.h"


TutorialTrigger::TutorialTrigger()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	// 튜토리얼 UI 노출
	Tutorial* tu = new Tutorial();

	_tutorial = tu;
	_tutorial->SetVisible(false);
	_tutorial->SetPos({ 300,50 });
	scene->AddUI(_tutorial);
}

TutorialTrigger::~TutorialTrigger()
{

}

void TutorialTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void TutorialTrigger::Tick()
{
	Super::Tick();

	if (_tutorial == nullptr)
		return;

	TouchInteract(_tutorial);
}