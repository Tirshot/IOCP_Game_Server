#include "pch.h"
#include "TriggerActor.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "UI.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "TimeManager.h"

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

	_scene = GET_SINGLE(SceneManager)->GetDevScene();
	_myPlayer = dynamic_cast<MyPlayer*>(_scene->GetCreatureAt({ info.posx(),info.posy() }));
}

void TriggerActor::PressSpaceInteract(UI* ui)
{ 
	if (_myPlayer)
	{
		Protocol::DIR_TYPE dir = _myPlayer->info.dir();

		// 위를 쳐다보고 Spacebar를 누르면 활성화
		if (_myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& _myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			ui->SetVisible(_visiblity);
		}
	}
	// 2초 후 숨김
	else
	{
		FadeOut();
		ui->SetVisible(_visiblity);
	}
}

void TriggerActor::TouchInteract(UI* ui)
{
	if (IsTouched())
	{
		_visiblity = true;
		ui->SetVisible(_visiblity);
	}
	// 해당 타일을 벗어나면 사라짐
	else
	{
		_visiblity = false;
		ui->SetVisible(_visiblity);
	}
}

bool TriggerActor::IsTouched()
{
	// myPlayer가 트리거 위에 있으면 활성화
	if (_myPlayer)
	{
		return true;
	}

	return false;
}

void TriggerActor::FadeOut()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (_visiblity)
	{
		_exptime += deltaTime;
	}

	if (_sumTime >= 2.f)
	{
		_visiblity = false;
		_exptime = 0.f;
	}
}

