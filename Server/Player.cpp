#include "pch.h"
#include "Player.h"
#include "GameRoom.h"

Player::Player()
{
	info.set_name("PlayerName");
	info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);
	info.set_hp(60);
	info.set_maxhp(60);
	info.set_attack(15);
	info.set_defence(0);
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	switch (info.state())
	{
	case IDLE:
		UpdateIdle();
		break;

	case MOVE:
		UpdateMove();
		break;

	case SKILL:
		UpdateSkill();
		break;
	}
}

void Player::UpdateIdle()
{

}

void Player::UpdateMove()
{
	SetState(IDLE);
}

void Player::UpdateSkill()
{
	if (room == nullptr)
		return;

	if (this->info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
	{
		// 내 앞에 있는 좌표
		GameObjectRef creature = room->GetGameObjectAt(GetFrontCellPos());
		if (creature)
		{
			creature->OnDamaged(shared_from_this());
		}
	}
	/*
	else if (_weaponType == WeaponType::Bow)
	{
		Arrow* arrow = scene->SpawnObject<Arrow>(GetCellPos());
		arrow->SetDir(info.dir());
		arrow->SetOwner(this);
	}*/
	SetState(IDLE);
}
