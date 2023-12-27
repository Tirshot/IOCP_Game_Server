#include "pch.h"
#include "HP.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

HP::HP()
{
	_blackHeart = GET_SINGLE(ResourceManager)->GetSprite(L"BlackHeart");
	_heart = GET_SINGLE(ResourceManager)->GetSprite(L"Heart");
}

HP::~HP()
{
	RemoveChild(this);
}

void HP::BeginPlay()
{
	AddChild(this);
	_hp = 5;
	_maxHp = 5;
}

void HP::Tick()
{
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	// MyPlayer의 체력에 따라 아이콘이 달라짐 -> 플립북을 사용해야 할 수도 있음
	if (_myPlayer)
	{
		_hp = _myPlayer->info.hp();
		_maxHp = _myPlayer->info.maxhp();
	}
}

void HP::Render(HDC hdc)
{
	for (int i = 0; i < _maxHp; i++)
	{	// Black Heart
		TransparentBlt(hdc,
			10 + 25 * i,
			10,
			25,
			21,
			_blackHeart->GetDC(),
			0,
			0,
			25,
			21,
			_blackHeart->GetTransparent());

		
	}

	for (int i = 0; i < _hp; i++)
	{
		// Heart
		TransparentBlt(hdc,
			10 + 25 * i,
			10,
			25,
			21,
			_heart->GetDC(),
			25,
			0,
			25,
			21,
			_heart->GetTransparent());
	}
}
