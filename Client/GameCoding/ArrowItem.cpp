#include "pch.h"
#include "ArrowItem.h"
#include "Sprite.h"
#include "TriggerActor.h"
#include "MyPlayer.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

ArrowItem::ArrowItem()
{
	_flipbookArrow = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowItem");
}

ArrowItem::~ArrowItem()
{

}

void ArrowItem::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(_flipbookArrow);

	TriggerActor* ta = new TriggerActor();
	ta->info.set_posx(info.posx());
	ta->info.set_posy(info.posy());
	_trigger = ta;
}

void ArrowItem::Tick()
{
	Super::Tick();
}

void ArrowItem::Render(HDC hdc)
{
	Super::Render(hdc);
}

void ArrowItem::TickIdle()
{
	// Item은 기본적으로 IDLE 상태이다
	if (_trigger->IsTouched())
	{
		DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
		MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

		if (scene == nullptr || myPlayer == nullptr)
			return;

		if (info.itemtype() == Protocol::ITEM_TYPE_ARROW)
			myPlayer->info.set_arrows(myPlayer->info.arrows() + 1);

		scene->RemoveActor(this);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		// 오브젝트 제거 패킷 전송
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_RemoveObject(this->info.objectid());
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
	}
}