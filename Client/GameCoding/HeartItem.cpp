#include "pch.h"
#include "HeartItem.h"
#include "Sprite.h"
#include "TriggerActor.h"
#include "MyPlayer.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

HeartItem::HeartItem()
{
	_flipbookHeart = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_HeartItem");
	_flipbookFull = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_FullHeartItem");
	_flipbookMax = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MaxHeartItem");
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

		if (scene == nullptr || myPlayer == nullptr)
			return;

		if (info.itemtype() == Protocol::ITEM_TYPE_HEART)
			myPlayer->info.set_hp(clamp(myPlayer->info.hp() + 1, 0, myPlayer->info.maxhp()));

		else if (info.itemtype() == Protocol::ITEM_TYPE_FULLHEART)
			myPlayer->info.set_hp(clamp(myPlayer->info.hp() + 999, 0, myPlayer->info.maxhp()));

		else if (info.itemtype() == Protocol::ITEM_TYPE_MAXHEART)
		{
			myPlayer->info.set_maxhp(myPlayer->info.maxhp() + 1);
			GET_SINGLE(ChatManager)->AddMessage(L"최대 체력이 1 증가했습니다.");
		}
		scene->RemoveActor(this);
		
		// 오브젝트 제거 패킷 전송
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_RemoveObject(this->info.objectid());
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		GET_SINGLE(SoundManager)->Play(L"Heart");
	}
}

void HeartItem::UpdateAnimation()
{
	switch (info.itemtype())
	{
	case Protocol::ITEM_TYPE_HEART:
		SetFlipbook(_flipbookHeart);
	break;

	case Protocol::ITEM_TYPE_FULLHEART:
		SetFlipbook(_flipbookFull);
	break;

	case Protocol::ITEM_TYPE_MAXHEART:
		SetFlipbook(_flipbookMax);
		break;

	default:
		return;
	}
}