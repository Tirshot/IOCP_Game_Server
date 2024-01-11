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

		// ���� �Ĵٺ��� Spacebar�� ������ Ȱ��ȭ
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			ui->SetVisible(_visiblity);
			GET_SINGLE(ChatManager)->AddMessage(L"â�� 2�� �Ŀ� �ڵ� ��Ȱ��ȭ�˴ϴ�.");
		}
	}
	// 2�� �� ����
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

		// ���� �Ĵٺ��� Spacebar�� ������ Ȱ��ȭ
		if (myPlayer->info.dir() == Protocol::DIR_TYPE_UP
			&& myPlayer->GetState() == Protocol::OBJECT_STATE_TYPE_SKILL
			&& GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			_visiblity = true;
			/*ui->SetVisible(_visiblity);*/
			GET_SINGLE(ChatManager)->AddMessage(L"â�� 2�� �Ŀ� �ڵ� ��Ȱ��ȭ�˴ϴ�.");
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

	// myPlayer�� Ʈ���� ���� ������ Ȱ��ȭ
	if (myPlayer)
	{
		return true;
	}

	return false;
}

