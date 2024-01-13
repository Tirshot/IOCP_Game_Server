#include "pch.h"
#include "TeleportEffect.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

TeleportEffect::TeleportEffect()
{
	SetLayer(LAYER_EFFECT);
}

TeleportEffect::~TeleportEffect()
{
}

void TeleportEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateAnimation();
}

void TeleportEffect::Tick()
{
	Super::Tick();

	// �ִϸ��̼� ���� �� ����Ʈ ����
	if (IsAnimationEnded())
	{
		Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		scene->RemoveActor(this);
	}
}

void TeleportEffect::Render(HDC hdc)
{
	Super::Render(hdc);
}

void TeleportEffect::UpdateAnimation()
{
	SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Teleport"));
}
