#include "pch.h"
#include "Snake.h"

Snake::Snake()
{
	info.set_name("Snake");
	info.set_monstertype(Protocol::MONSTER_TYPE_SNAKE);
	info.set_hp(30);
	info.set_maxhp(30);
	info.set_attack(10);
	info.set_defence(0);
	info.set_speed(160);
}

Snake::~Snake()
{
}

void Snake::Init()
{
	Super::Init();
}

void Snake::Update()
{
	Super::Update();
}

void Snake::UpdateIdle()
{
	Super::UpdateIdle();
}

void Snake::UpdateMove()
{
	Super::UpdateMove();
}

void Snake::UpdateSkill()
{
	Super::UpdateSkill();
}

void Snake::UpdateHit()
{
	Super::UpdateHit();
}
