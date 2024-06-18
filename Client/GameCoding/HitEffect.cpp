#include "pch.h"
#include "HitEffect.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

HitEffect::HitEffect()
{
	SetLayer(LAYER_EFFECT);
}

HitEffect::~HitEffect()
{
}

void HitEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateAnimation();
}

void HitEffect::Tick()
{
	Super::Tick();

	// �ִϸ��̼� ���� �� ����Ʈ ����
	if (IsAnimationEnded())
	{
		auto scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		scene->RemoveActor(shared_from_this());
	}
}

void HitEffect::Render(HDC hdc)
{
	Super::Render(hdc);
}

void HitEffect::UpdateAnimation()
{
	SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Hit"));
}
