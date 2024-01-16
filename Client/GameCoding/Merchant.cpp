#include "pch.h"
#include "Merchant.h"
#include "DevScene.h"
#include "Flipbook.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"
#include "TutorialTrigger.h"

Merchant::Merchant()
{
	// ¸»Ç³¼± ³ª¿À´Â ÇÃ¸³ºÏ
	_flipbookIdle = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MerchantIdle");

	// ¸»Ç³¼± ¾ø´Â ÇÃ¸³ºÏ
	_flipbookMove = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MerchantMove");
}

Merchant::~Merchant()
{
}

void Merchant::BeginPlay()
{
	Super::BeginPlay();
}

void Merchant::Tick()
{
	Super::Tick();

	UpdateAnimation();
}

void Merchant::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Merchant::TickIdle()
{
	Scene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(scene->GetCreatureAt(GetFrontCellPos()));

		if (myPlayer)
		{
			SetState(MOVE);
		}
	}
}

void Merchant::TickMove()
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

void Merchant::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove);
		break;
	}
}
