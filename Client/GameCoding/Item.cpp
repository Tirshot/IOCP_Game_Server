#include "pch.h"
#include "Item.h"
#include "Sprite.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "ResourceManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "SceneManager.h"

Item::Item()
{
	SetLayer(LAYER_BACKGROUND);
	SetState(IDLE);
}

Item::~Item()
{
}

void Item::BeginPlay()
{
	Super::BeginPlay();
}

void Item::Tick()
{
	Super::Tick();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (scene == nullptr)
		return;

	if (myPlayer == nullptr)
		return;


	int itemID = itemInfo.itemid();
	auto& item = GET_SINGLE(ItemManager)->GetItem(itemID);

	_name = item.KorName;
	_sprite = GET_SINGLE(ItemManager)->GetSprite(itemID);

	int count = itemInfo.itemcount();
	string name = myPlayer->info.name();
	wstring nameWstr = GET_SINGLE(ChatManager)->StringToWStr(name);

	// 플레이어와 드랍된 이 아이템과 접촉하면
	if (myPlayer->GetCellPos() == GetCellPos())
	{
		// 아이템 효과 적용
		switch (itemID)
		{
		case 6:	// MaxHeart
			myPlayer->info.set_maxhp(clamp(myPlayer->info.maxhp() + 1, 0, 10));
			GET_SINGLE(ChatManager)->AddMessage(L"최대 체력이 1 증가했습니다.");
			break;

		case 7:	// Heart
			myPlayer->info.set_hp(clamp(myPlayer->info.hp() + 1, 0, myPlayer->info.maxhp()));
			GET_SINGLE(ChatManager)->AddMessage(L"체력이 1 회복 되었습니다.");
			break;

		default:	// 습득 가능한 아이템
			GET_SINGLE(ItemManager)->AddItemToInventory(itemID);
			GET_SINGLE(ChatManager)->AddMessage(_name + L" 아이템 획득");
			break;
		}

		// 아이템 제거
		scene->RemoveActor(this);
	}
}

void Item::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_sprite == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	::TransparentBlt(hdc,
		(int32)_pos.x - ((int32)_sprite->GetSize().x / 2) - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - ((int32)cameraPos.y - GWinSizeY / 2),
		32,
		32,
		_sprite->GetDC(),
		0,
		0,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());
}

void Item::TickIdle()
{

}