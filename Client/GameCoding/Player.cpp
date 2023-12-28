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
#include "Monster.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

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

	//���� �������� �޾ƿ�
	//info.set_hp(100);
	//info.set_maxhp(100);
	//info.set_attack(15);
	//info.set_defence(0);
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

}

void Player::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 1.f )
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 160 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 160 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 160 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 160 * deltaTime;
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
		// ���� ����
		DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
		if (scene == nullptr)
			return;

		if (info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
		{
			// �� �տ� �ִ� ��ǥ
			Creature* creature = scene->GetCreatureAt(GetFrontCellPos());

			if (creature)
			{
				if (creature->GetType() == Protocol::OBJECT_TYPE_PLAYER)
				{
					SetState(IDLE);
					return;
				}
				// ���Ͱ� �÷��̾�� �ǰ�
				creature->OnDamaged(this);
				creature->SetState(HIT);
				creature->SetWait(300);
			}
		}
		else if (GetWeaponType() == Protocol::WEAPON_TYPE_BOW)
		{
			// ȭ�� ���� ��Ŷ ����
			{
				SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Fire(GetObjectID());
				GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			}
		}
		SetState(IDLE);
	}
}

void Player::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;

	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case SKILL:
		if (GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
		{
			SetFlipbook(_flipbookAttack[info.dir()]);
		}
		else if (GetWeaponType() == Protocol::WEAPON_TYPE_BOW)
		{
			SetFlipbook(_flipbookBow[info.dir()]);
		}
		else
		{
			SetFlipbook(_flipbookStaff[info.dir()]);
		}
		break;

	default:
		return;
	}
}

void Player::Handle_S_Fire(const Protocol::ObjectInfo& info, uint64 id)
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	{
		Arrow* arrow = scene->SpawnObject<Arrow>(GetCellPos());
		arrow->SetDir(info.dir());
		arrow->SetOwner(this);
		arrow->info = info;
	}
}
