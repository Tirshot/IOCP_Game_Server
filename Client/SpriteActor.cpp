#include "pch.h"
#include "SpriteActor.h"
#include "Sprite.h"
#include "SceneManager.h"

SpriteActor::SpriteActor()
{
}

SpriteActor::~SpriteActor()
{
}

void SpriteActor::BeginPlay()
{
	// 부모의 BeginPlay 호출
	Super::BeginPlay();

}

void SpriteActor::Tick()
{
	// 부모의 Tick 호출
	Super::Tick();

}

void SpriteActor::Render(HDC hdc)
{
	// 부모의 Render 호출
	Super::Render(hdc);

	if (_sprite == nullptr)
		return;
	
	Vec2Int size = _sprite->GetSize();

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	//::TransparentBlt(hdc,							// 처음 위치
	//	(int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),	// 액터의 중앙 - 카메라 위치
	//	(int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
	//	size.x,
	//	size.y,
	//	_sprite->GetDC(),				// Source
	//	_sprite->GetPos().x,
	//	_sprite->GetPos().y,
	//	_sprite->GetSize().x,			// 얼마만큼 더 잘라서 사용할 것인가?
	//	_sprite->GetSize().y,
	//	_sprite->GetTransparent());

	::BitBlt(hdc,
		(int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		SRCCOPY);
}
