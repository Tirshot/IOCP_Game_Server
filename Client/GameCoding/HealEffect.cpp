#include "pch.h"
#include "HealEffect.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

HealEffect::HealEffect()
{
	SetLayer(LAYER_EFFECT);
}

HealEffect::~HealEffect()
{
}

void HealEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateAnimation();
}

void HealEffect::Tick()
{
	Super::Tick();

	// �ִϸ��̼� ���� �� ����Ʈ ����
	if (IsAnimationEnded())
	{
		Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		scene->RemoveActor(this);
	}
}

void HealEffect::Render(HDC hdc)
{
	Super::Render(hdc);
}

void HealEffect::UpdateAnimation()
{
	SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Heal"));
}
