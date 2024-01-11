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
}

void TriggerActor::PressSpaceInteract(UI* ui)
{ 
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt({ info.posx(),info.posy() }));

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (myPlayer)
	{
		Protocol::DIR_TYPE dir = myPlayer->info.dir();

		// 위를 쳐다보고 Spacebar를 누르면 활성화
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			ui->SetVisible(_visiblity);
			GET_SINGLE(ChatManager)->AddMessage(L"창은 2초 후에 자동 비활성화됩니다.");
		}
	}
	// 2초 후 숨김
	if (_visiblity)
	{
		_exptime += deltaTime;
	}

	if (_exptime >= 2.f)
	{
		_visiblity = false;
		ui->SetVisible(_visiblity);
		_exptime = 0.f;
	}
}

void TriggerActor::PressSpaceInteract()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt({ info.posx(),info.posy() }));

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (myPlayer)
	{
		Protocol::DIR_TYPE dir = myPlayer->info.dir();

		// 위를 쳐다보고 Spacebar를 누르면 활성화
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			/*ui->SetVisible(_visiblity);*/
			GET_SINGLE(ChatManager)->AddMessage(L"창은 2초 후에 자동 비활성화됩니다.");
		}
	}

	if (_visiblity)
	{
		_exptime += deltaTime;
	}

	if (_sumTime >= 2.f)
	{
		_visiblity = false;
		/*ui->SetVisible(_visiblity);*/
		_exptime = 0.f;
	}
}

bool TriggerActor::IsTouched()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt({ info.posx(),info.posy() }));

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	// myPlayer가 트리거 위에 있으면 활성화
	if (myPlayer)
	{
		return true;
	}

	return false;
}

