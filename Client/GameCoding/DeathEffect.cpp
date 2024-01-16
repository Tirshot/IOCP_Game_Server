#include "pch.h"
#include "DeathEffect.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

DeathEffect::DeathEffect()
{
	SetLayer(LAYER_EFFECT);
}

DeathEffect::~DeathEffect()
{
}

void DeathEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateAnimation();
}

void DeathEffect::Tick()
{
	Super::Tick();

	// �ִϸ��̼� ���� �� ����Ʈ ����
	if (IsAnimationEnded())
	{
		Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		scene->RemoveActor(this);
	}
}

void DeathEffect::Render(HDC hdc)
{
	Super::Render(hdc);
}

void DeathEffect::UpdateAnimation()
{
	SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Death"));
}
