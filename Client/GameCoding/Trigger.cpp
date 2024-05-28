#include "Trigger.h"
#include "pch.h"
#include "Trigger.h"
#include "UI.h"
#include "Panel.h"
#include "SceneManager.h"

Trigger::Trigger()
{

}

Trigger::~Trigger()
{

}

void Trigger::BeginPlay()
{
	Super::BeginPlay();
}

void Trigger::Tick()
{
	Super::Tick();

	// 바라보고 스페이스를 입력하면 ui가 표시
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (_ui == nullptr)
		return;

	if (scene)
	{
		UI* ui = dynamic_cast<UI*>(_ui);
		if (ui)
		{
			PressSpaceInteract(_ui);
		}

		//// 움직이면 UI 숨김
		//if (IsTouched() == false)
		//{
		//	shopUI->SetVisible(false);
		//	shopUI->SetChildInvisible();
		//	shopUI->ResetInitializingTime();
		//}
	}
}
