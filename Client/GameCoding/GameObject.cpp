#include "pch.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "CameraComponent.h"
#include "BoxCollider.h"
#include "SceneManager.h"
#include "DevScene.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void GameObject::Tick()
{
	_dirtyFlag = false;

	Super::Tick();

	switch (info.state())
	{
	case IDLE:
		TickIdle();
		break;
	case MOVE:
		TickMove();
		break;
	case SKILL:
		TickSkill();
		break;
	
		// Projectiles only state
	case HIT:
		TickHit();
		break;

		// MyPlayer only state
	case CHAT:
		TickChat();
		break;

	case SPIN_READY:
		TickSpinReady();
		break;

	case SPIN:
		TickSpin();
		break;

	case TELEPORT:
		TickTeleport();
		break;
	}
}

void GameObject::Render(HDC hdc)
{
	Super::Render(hdc);
}

void GameObject::SetState(ObjectState state)
{
	if (info.state() == state)
		return;

	info.set_state(state);
	UpdateAnimation();

	_dirtyFlag = true;
}

void GameObject::SetDir(Dir dir)
{
	info.set_dir(dir);
	UpdateAnimation();

	_dirtyFlag = true;
}

bool GameObject::HasReachedDest()
{
	Vec2 dir = (_destPos - _pos);
	return (dir.Length() < 10.f);
}

bool GameObject::CanGo(Vec2Int cellPos)
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return false;

	// 몬스터와 충돌
	if (this->info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
		return scene->MonsterCanGo(cellPos);

	return scene->CanGo(cellPos);
}

Dir GameObject::GetLookAtDir(Vec2Int cellPos)
{
	Vec2Int dir = cellPos - GetCellPos();
	if (dir.x > 0)
		return DIR_RIGHT;
	else if (dir.x < 0)
		return DIR_LEFT;
	else if (dir.y > 0)
		return DIR_DOWN;
	else
		return DIR_UP;
}

void GameObject::SetCellPos(Vec2Int cellPos, bool teleport)
{
	info.set_posx(cellPos.x);
	info.set_posy(cellPos.y);

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene == nullptr)
		return;

	_destPos = scene->ConvertPos(cellPos);

	if (teleport)
		_pos = _destPos;

	_dirtyFlag = true;
}

Vec2Int GameObject::GetCellPos()
{
	return Vec2Int(info.posx(), info.posy());
}

Vec2Int GameObject::GetFrontCellPos()
{
	switch (info.dir())
	{
	case DIR_DOWN:
		return GetCellPos() + Vec2Int{0,1};
	case DIR_LEFT:
		return GetCellPos() + Vec2Int{ -1,0 };
	case DIR_RIGHT:
		return GetCellPos() + Vec2Int{ 1,0 };
	case DIR_UP:
		return GetCellPos() + Vec2Int{ 0,-1 };
	}

	return GetCellPos();
}

Vec2Int GameObject::GetBehindCellPos()
{
	switch (info.dir())
	{
	case DIR_DOWN:
		return GetCellPos() + Vec2Int{ 0,-1 };
	case DIR_LEFT:
		return GetCellPos() + Vec2Int{ 1,0 };
	case DIR_RIGHT:
		return GetCellPos() + Vec2Int{ -1,0 };
	case DIR_UP:
		return GetCellPos() + Vec2Int{ 0,1 };
	}

	return GetCellPos();
}

void GameObject::SetWeaponType(Protocol::WEAPON_TYPE weaponType)
{
	info.set_weapontype(weaponType);
	UpdateAnimation();

	_dirtyFlag = true;
}

