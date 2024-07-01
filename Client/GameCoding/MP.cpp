#include "pch.h"
#include "MP.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "Texture.h"

MP::MP()
{
	_blackMp = GET_SINGLE(ResourceManager)->GetTexture(L"BlackMp");
	_blueMp = GET_SINGLE(ResourceManager)->GetTexture(L"MP");

	_pos = { 10, 35 };
}

MP::~MP()
{
}

void MP::BeginPlay()
{

}

void MP::Tick()
{
	_deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	// MyPlayer의 체력에 따라 아이콘이 달라짐 -> 플립북을 사용해야 할 수도 있음
	if (_myPlayer)
	{
		_mp = clamp(_myPlayer->info.mp(), 0, _myPlayer->info.maxmp());
		_maxMp = _myPlayer->info.maxmp();
	}
}

void MP::Render(HDC hdc)
{
	{	// Black Mp, 80 x 20
		TransparentBlt(hdc,
			_pos.x,
			_pos.y,
			_maxMp * 2.5f,
			20,
			_blackMp->GetDC(),
			0,
			0,
			80,
			20,
			RGB(255,0,255));
	}

	{	// Mp, 80 x 20
		TransparentBlt(hdc,
			_pos.x,
			_pos.y,
			_mp * 2.5f,
			20,
			_blueMp->GetDC(),
			0,
			0,
			80,
			20,
			RGB(255, 0, 255));
	}
}
