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

		// ���� �Ĵٺ��� Spacebar�� ������ Ȱ��ȭ
		if (_myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& _myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			ui->SetVisible(_visiblity);
		}
	}
	// 2�� �� ����
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
	// �ش� Ÿ���� ����� �����
	else
	{
		_visiblity = false;
		ui->SetVisible(_visiblity);
	}
}

bool TriggerActor::IsTouched()
{
	// myPlayer�� Ʈ���� ���� ������ Ȱ��ȭ
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

