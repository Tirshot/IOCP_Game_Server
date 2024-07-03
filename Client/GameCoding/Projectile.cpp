#include "pch.h"
#include "Projectile.h"
#include "Creature.h"
#include "Arrow.h"

Projectile::Projectile()
{
	SetLayer(LAYER_EFFECT);
	info.set_objecttype(Protocol::OBJECT_TYPE_PROJECTILE);
}

Projectile::~Projectile()
{
}

void Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void Projectile::Tick()
{
	Super::Tick();
}

void Projectile::Render(HDC hdc)
{
	Super::Render(hdc);
}
