#include "pch.h"
#include "MerchantTrigger.h"
#include "DevScene.h"
#include "Merchant.h"
#include "MerchantUI.h"
#include "ResourceManager.h"
#include "SceneManager.h"

MerchantTrigger::MerchantTrigger()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	// ���� panel ����
	MerchantUI* tu = new MerchantUI();

	_merchantUI = tu;
	_merchantUI->SetVisible(false);
	_merchantUI->SetPos({ 300,50 });
	scene->AddUI(_merchantUI);
}

MerchantTrigger::~MerchantTrigger()
{
}

void MerchantTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void MerchantTrigger::Tick()
{
	Super::Tick();

	if (_merchantUI == nullptr)
		return;

	// �ٶ󺸰� �����̽��� �Է��ϸ� ui�� ǥ��
	PressSpaceInteract(_merchantUI);
}
