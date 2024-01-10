#include "pch.h"
#include "Sign.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"
#include "TutorialTrigger.h"

Sign::Sign()
{
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SignIdle");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SignMove");
}

Sign::~Sign()
{
}

void Sign::BeginPlay()
{
	Super::BeginPlay();
}

void Sign::Tick()
{
	Super::Tick();

	UpdateAnimation();
}

void Sign::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Sign::TickIdle()
{
	// 매 틱마다 표지판 앞에 MyPlayer가 있는지 체크, DIR_UP도 체크
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt(GetFrontCellPos()));

		if (myPlayer)
		{
			SetState(MOVE);
		}
	}
}

void Sign::TickMove()
{
	Scene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt(GetFrontCellPos()));

		if (myPlayer == nullptr)
		{
			SetState(IDLE);
		}
	}
}

void Sign::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[DIR_DOWN]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[DIR_DOWN]);
		break;
	}
}
