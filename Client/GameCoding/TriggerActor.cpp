#include "pch.h"
#include "TriggerActor.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "UI.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

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
	if (IsTouched())
	{
		MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

		Protocol::DIR_TYPE dir = myPlayer->info.dir();

		// ���� �Ĵٺ��� Spacebar�� ������ Ȱ��ȭ
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			GET_SINGLE(SoundManager)->Play(L"Merchant");
			_visiblity = true;
			_isTouched = true;
			ui->SetVisible(_visiblity);
		}
	}
	else
	{
		_visiblity = false;
		_isTouched = false;
		ui->SetVisible(_visiblity);
	}
}

void TriggerActor::TouchInteract(UI* ui)
{
	if (IsTouched())
	{
		_visiblity = true;
		_isTouched = true;
		ui->SetVisible(_visiblity);
	}
	// �ش� Ÿ���� ����� �����
	else
	{
		_visiblity = false;
		_isTouched = false;
		ui->SetVisible(_visiblity);
	}
}

bool TriggerActor::IsTouched()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt({ info.posx(),info.posy() }));

	if (myPlayer)
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

