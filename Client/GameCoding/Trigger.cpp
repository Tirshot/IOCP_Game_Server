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

	// �ٶ󺸰� �����̽��� �Է��ϸ� ui�� ǥ��
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (_ui == nullptr)
		return;

	if (scene)
	{
		auto ui = dynamic_pointer_cast<UI>(_ui);
		if (ui)
		{
			PressSpaceInteract(_ui);
		}

		//// �����̸� UI ����
		//if (IsTouched() == false)
		//{
		//	shopUI->SetVisible(false);
		//	shopUI->SetChildInvisible();
		//	shopUI->ResetInitializingTime();
		//}
	}
}
