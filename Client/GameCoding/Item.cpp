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
	SetLayer(LAYER_OBJECT);
	SetState(IDLE);
}

Item::~Item()
{
	itemInfo = {};
}

void Item::BeginPlay()
{
	Super::BeginPlay();

	info.set_objectid(itemInfo.objectid());

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (scene == nullptr)
		return;

	if (myPlayer == nullptr)
		return;

	int itemID = itemInfo.itemid();
	auto item = GET_SINGLE(ItemManager)->GetItem(itemID);

	_name = GET_SINGLE(ChatManager)->StringToWStr(itemInfo.itemname());
	_sprite = GET_SINGLE(ItemManager)->GetSprite(itemID);
	_korName = item.KorName;
}

void Item::Tick()
{
	Super::Tick();

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (scene == nullptr)
		return;

	if (myPlayer == nullptr)
		return;

	auto sprites = GET_SINGLE(ResourceManager)->GetSprites();

	// 플레이어와 드랍된 이 아이템과 접촉하면
	if (myPlayer->GetCellPos() == GetCellPos())
	{
		// 아이템 효과 적용
		switch (itemInfo.itemid())
		{
		case 6:	// MaxHeart
			myPlayer->info.set_maxhp(clamp(myPlayer->info.maxhp() + MAX_HP_AMOUNT, 0, MAX_HP_AMOUNT * 10));
			GET_SINGLE(ChatManager)->AddMessage(L"최대 체력이 한 칸 증가했습니다.");
			break;

		case 7:	// Heart
			myPlayer->info.set_hp(clamp(myPlayer->info.hp() + myPlayer->GetPotionEffect() * 2, 0, myPlayer->info.maxhp()));
			GET_SINGLE(ChatManager)->AddMessage(L"체력이 10 회복 되었습니다.");
			break;

		default:	// 습득 가능한 아이템
			GET_SINGLE(ItemManager)->AddItemToInventory(itemInfo.itemid(), itemInfo.itemcount());
			GET_SINGLE(ChatManager)->AddMessage(_korName + L" 아이템 "+ to_wstring(itemInfo.itemcount()) + L"개 획득");
			break;
		}

		// 아이템 제거
		scene->RemoveActor(shared_from_this());
	}
}

void Item::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_sprite == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	::TransparentBlt(hdc,
		(int32)_pos.x - ((int32)cameraPos.x - GWinSizeX / 2) - 16,
		(int32)_pos.y - ((int32)cameraPos.y - GWinSizeY / 2),
		32,
		32,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());
}

void Item::TickIdle()
{

}