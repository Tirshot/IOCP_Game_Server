#include "pch.h"
#include "MerchantTrigger.h"
#include "DevScene.h"
#include "Merchant.h"
#include "MerchantUI.h"
#include "ShopUI.h"
#include "ResourceManager.h"
#include "SceneManager.h"

MerchantTrigger::MerchantTrigger()
{
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

	// �ٶ󺸰� �����̽��� �Է��ϸ� ui�� ǥ��
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{	
		MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
		ShopUI* shopUI = scene->FindUI<ShopUI>(scene->GetUIs());

		if(merchantUI)
			PressSpaceInteract(merchantUI);

		// �����̸� UI ����
		if (IsTouched() == false)
		{
			shopUI->SetVisible(false);
			//questUI
		}
	}
}
