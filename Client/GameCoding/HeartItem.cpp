#include "pch.h"
#include "HeartItem.h"
#include "Sprite.h"
#include "TriggerActor.h"
#include "MyPlayer.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"

HeartItem::HeartItem()
{
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_HeartItem");
}

HeartItem::~HeartItem()
{

}

void HeartItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerActor* ta = new TriggerActor();
	ta->info.set_posx(info.posx());
	ta->info.set_posy(info.posy());
	_trigger = ta;
}

void HeartItem::Tick()
{
	Super::Tick();

	UpdateAnimation();
}

void HeartItem::Render(HDC hdc)
{
	Super::Render(hdc);
}

void HeartItem::TickIdle()
{
	// Item은 기본적으로 IDLE 상태이다
	if (_trigger->IsTouched())
	{
		DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
		MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
		myPlayer->info.set_hp(clamp(myPlayer->info.hp() + 1, 0, myPlayer->info.maxhp()));
		scene->RemoveActor(this);
	}
}

void HeartItem::UpdateAnimation()
{
	if (_flipbook == _flipbookIdle[DIR_DOWN])
		return;

	SetFlipbook(_flipbookIdle[DIR_DOWN]);
}