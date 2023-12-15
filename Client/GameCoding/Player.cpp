#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "Arrow.h"

Player::Player()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_flipbookAttack[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackUp");
	_flipbookAttack[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackDown");
	_flipbookAttack[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackLeft");
	_flipbookAttack[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackRight");

	_flipbookBow[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowUp");
	_flipbookBow[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowDown");
	_flipbookBow[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowLeft");
	_flipbookBow[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowRight");

	_flipbookStaff[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffUp");
	_flipbookStaff[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffDown");
	_flipbookStaff[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffLeft");
	_flipbookStaff[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffRight");

	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);

	_type = CreatureType::Player;

	_stat.hp = 100;
	_stat.maxHp = 100;
	_stat.attack = 30;
	_stat.defence = 5;
}

Player::~Player()
{

}

void Player::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void Player::Tick()
{
	Super::Tick();

}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::TickIdle()
{

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_keyPressed = true;
	// deltaXY = {위, 아래, 왼쪽, 오른쪽}
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	// 공격
	if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar))
	{
		SetState(SKILL);
	}

	else if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
	{
		SetDir(DIR_UP);

		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else  if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
	{
		SetDir(DIR_DOWN);

		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::A))
	{
		SetDir(DIR_LEFT);

		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::D))
	{
		SetDir(DIR_RIGHT);

		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else
	{ // 갈 수 없는 경우에는 애니메이션만 실행
		_keyPressed = false;
		if (info.state() == IDLE)
			UpdateAnimation();
	}
	
	// 무기 선택, UI 연동 필요
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1))
	{
		SetWeaponType(WeaponType::Sword);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(WeaponType::Bow);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3))
	{
		// 추후에 갈고리로 변경예정
		SetWeaponType(WeaponType::Staff);
	}


}

void Player::TickMove()
{

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 4.f )
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 200 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 200 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 200 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 200 * deltaTime;
			break;
		}
	}
}

void Player::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (IsAnimationEnded())
	{
		// 공격 판정
		DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
		if (scene == nullptr)
			return;

		if (_weaponType == WeaponType::Sword)
		{
			// 내 앞에 있는 좌표
			Creature* creature = scene->GetCreatureAt(GetFrontCellPos());
			if (creature)
			{
				scene->SpawnObject<HitEffect>(GetFrontCellPos());
				// 데미지 피격
				creature->OnDamaged(this);
			}
		}
		else if (_weaponType == WeaponType::Bow)
		{
			Arrow* arrow = scene->SpawnObject<Arrow>(GetCellPos());
			arrow->SetDir(info.dir());
			arrow->SetOwner(this);
		}

		SetState(IDLE);
	}
}

void Player::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		if (_keyPressed)
			SetFlipbook(_flipbookMove[info.dir()]);
		else
			SetFlipbook(_flipbookIdle[info.dir()]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case SKILL:
		if (_weaponType == WeaponType::Sword)
			SetFlipbook(_flipbookAttack[info.dir()]);
		else if (_weaponType == WeaponType::Bow)
			SetFlipbook(_flipbookBow[info.dir()]);
		else
			SetFlipbook(_flipbookStaff[info.dir()]);
		break;
	}
}