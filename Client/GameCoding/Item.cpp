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

	// �÷��̾�� ����� �� �����۰� �����ϸ�
	if (myPlayer->GetCellPos() == GetCellPos())
	{
		// ������ ȿ�� ����
		switch (itemInfo.itemid())
		{
		case 6:	// MaxHeart
			myPlayer->info.set_maxhp(clamp(myPlayer->info.maxhp() + MAX_HP_AMOUNT, 0, MAX_HP_AMOUNT * 10));
			GET_SINGLE(ChatManager)->AddMessage(L"�ִ� ü���� �� ĭ �����߽��ϴ�.");
			break;

		case 7:	// Heart
			myPlayer->info.set_hp(clamp(myPlayer->info.hp() + myPlayer->GetPotionEffect() * 2, 0, myPlayer->info.maxhp()));
			GET_SINGLE(ChatManager)->AddMessage(L"ü���� 10 ȸ�� �Ǿ����ϴ�.");
			break;

		default:	// ���� ������ ������
			GET_SINGLE(ItemManager)->AddItemToInventory(itemInfo.itemid(), itemInfo.itemcount());
			GET_SINGLE(ChatManager)->AddMessage(_korName + L" ������ "+ to_wstring(itemInfo.itemcount()) + L"�� ȹ��");
			break;
		}

		// ������ ����
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