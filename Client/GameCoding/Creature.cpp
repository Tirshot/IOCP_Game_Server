#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "Monster.h"
#include "ClientPacketHandler.h"

Creature::Creature()
{
}

Creature::~Creature()
{
}

void Creature::BeginPlay()
{
	Super::BeginPlay();
}

void Creature::Tick()
{
	Super::Tick();
}

void Creature::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Creature::OnDamaged(Creature* attacker)
{
	if (attacker == nullptr)
		return;

	// disable PvP : 동족은 공격 불가
	if (GetType() == attacker->GetType())
		return;

	int32 damage = attacker->info.attack() - info.defence();

	if (damage <= 0)
		return;

	// hp는 항상 양수
	info.set_hp(max(0, info.hp() - damage));
	
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (this->GetType() == Protocol::OBJECT_TYPE_PLAYER)
	{
		scene->SpawnObject<HitEffect>(GetCellPos());
	}

	if (info.hp() == 0)
	{
		if (scene)
		{
			scene->RemoveActor(this);
		}
	}
}