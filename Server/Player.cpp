#include "pch.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Init()
{
	info.set_name("PlayerName");
	info.set_hp(100);
	info.set_maxhp(100);
	info.set_attack(15);
	info.set_defence(0);
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
	SetState(IDLE);
}
