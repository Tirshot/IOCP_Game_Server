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

	// 바라보고 스페이스를 입력하면 ui가 표시
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{	
		MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
		ShopUI* shopUI = scene->FindUI<ShopUI>(scene->GetUIs());

		if(merchantUI)
			PressSpaceInteract(merchantUI);

		// 움직이면 UI 숨김
		if (IsTouched() == false)
		{
			shopUI->SetVisible(false);
			//questUI
		}
	}
}
