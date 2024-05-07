#include "pch.h"
#include "MerchantTrigger.h"
#include "DevScene.h"
#include "Merchant.h"
#include "MyPlayer.h"
#include "MerchantUI.h"
#include "MerchantDialogueUI.h"
#include "ShopUI.h"
#include "QuestUI.h"
#include "ClientPacketHandler.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"

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

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());

	if (myPlayer)
	{
		// 캐릭터와 멀어지면 비활성화
		float length = (GetCellPos() - myPlayer->GetCellPos()).Length();
		if (length >= 2)
			return;

		if (length >= 1 && length < 2)
		{
			HideUIs();
			_isTouched = false;
			return;
		}

		if (_isTouched == false && scene && merchantUI)
		{
			// 바라보고 스페이스를 입력하면 ui가 표시
			PressSpaceInteract(merchantUI);
		}
	}
}

void MerchantTrigger::HideUIs()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
		ShopUI* shopUI = scene->FindUI<ShopUI>(scene->GetUIs());
		MerchantDialogueUI* dialogueUI = scene->FindUI<MerchantDialogueUI>(scene->GetUIs());
		QuestUI* questUI = scene->FindUI<QuestUI>(scene->GetUIs());

		if (merchantUI)
		{
			merchantUI->SetVisible(false);
		}

		if (shopUI)
		{
			shopUI->SetVisible(false);
			shopUI->ResetPos();
			shopUI->ResetInitializingTime();
			shopUI->ResetPage();
		}

		if (dialogueUI)
		{
			dialogueUI->SetVisible(false);
			dialogueUI->ResetPage();
		}

		if (questUI)
		{
			questUI->SetVisible(false);
			questUI->ResetPos();
			questUI->ResetPage();
		}
	}
}
