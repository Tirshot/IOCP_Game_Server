#include "pch.h"
#include "Moblin.h"

Moblin::Moblin()
{
	info.set_name("Moblin");
	info.set_monstertype(Protocol::MONSTER_TYPE_MOBLIN);
	info.set_hp(80);
	info.set_maxhp(80);
	info.set_attack(2);
	info.set_defence(0);
	info.set_speed(150);
}

Moblin::~Moblin()
{
}

void Moblin::Init()
{
	Super::Init();
}

void Moblin::Update()
{
	Super::Update();
}

void Moblin::UpdateIdle()
{
	Super::UpdateIdle();
}

void Moblin::UpdateMove()
{
	Super::UpdateMove();
}

void Moblin::UpdateSkill()
{
	Super::UpdateSkill();
}

void Moblin::UpdateHit()
{
	Super::UpdateHit();
}
