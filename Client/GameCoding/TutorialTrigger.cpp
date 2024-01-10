#include "pch.h"
#include "TutorialTrigger.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "Tutorial.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "TimeManager.h"

TutorialTrigger::TutorialTrigger()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

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

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt({ info.posx(),info.posy() }));
	
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (myPlayer)
	{
		Protocol::DIR_TYPE dir = myPlayer->info.dir();
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			_tutorial->SetVisible(_visiblity);
			GET_SINGLE(ChatManager)->AddMessage(L"창은 2초 후에 자동 비활성화됩니다.");
		}
	}

	if (_visiblity)
	{
		_sumTime += deltaTime;
	}

	if (_sumTime >= 2.f)
	{
		_visiblity = false;
		_tutorial->SetVisible(_visiblity);
		_sumTime = 0.f;
	}
}