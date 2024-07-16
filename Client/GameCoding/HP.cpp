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
	_pos = { 10, 10 };
}

HP::~HP()
{
	_myPlayer = nullptr;
	_blackHeart = nullptr;
	_heart = nullptr;
}

void HP::BeginPlay()
{
	_hp = 50;
	_maxHp = 50;
}

void HP::Tick()
{
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	// MyPlayer의 체력에 따라 아이콘이 달라짐 -> 플립북을 사용해야 할 수도 있음
	if (_myPlayer)
	{
		_hp = clamp(_myPlayer->info.hp(), 0, _myPlayer->info.maxhp());
		_maxHp = _myPlayer->info.maxhp();
	}
}

void HP::Render(HDC hdc)
{
	for (int i = 0; i < (_maxHp / 10); i++)
	{	// Black Heart
		TransparentBlt(hdc,
			_pos.x + 25 * i,
			_pos.y,
			25,
			21,
			_blackHeart->GetDC(),
			0,
			0,
			25,
			21,
			_blackHeart->GetTransparent());
	}

	for (int i = 0; i < (_hp / 10); i++)
	{
		// 일반적인 하트를 그릴 때
		TransparentBlt(hdc,
			_pos.x + 25 * i,
			_pos.y,
			25,
			21,
			_heart->GetDC(),
			25,
			0,
			25,
			21,
			_heart->GetTransparent());
	}

	// 마지막 하트를 부분적으로 그릴 때
	if (_hp % 10 != 0)
	{
		int remainHp = _hp % 10;
		int width = (remainHp * 25) / 10;

		TransparentBlt(hdc,
			_pos.x + 25 * (_hp / 10),
			_pos.y,
			width,
			21,
			_heart->GetDC(),
			25,
			0,
			width,
			21,
			_heart->GetTransparent());
	}
}
