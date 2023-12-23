#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "Monster.h"

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

	// disable PvP : ������ ���� �Ұ�
	if (GetType() == attacker->GetType())
		return;

	int32 damage = attacker->info.attack() - this->info.defence();

	if (damage <= 0)
		return;

	// hp�� �׻� ���
	info.set_hp(max(0, info.hp() - damage));
	
	// ���� �ǰ� �ִϸ��̼� ���
	if (attacker->GetType() == Protocol::OBJECT_TYPE_PLAYER)
		this->SetState(HIT);

	if (info.hp() == 0)
	{
		Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		if (scene)
		{
			scene->RemoveActor(this);
		}
	}
}