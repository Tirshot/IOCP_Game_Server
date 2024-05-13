#include "pch.h"
#include "DevScene.h"
#include "SceneManager.h"
#include "PopUp.h"

PopUp::PopUp()
{
}

PopUp::~PopUp()
{
}

void PopUp::BeginPlay()
{
	Super::BeginPlay();
}

void PopUp::Tick()
{

}

void PopUp::Render(HDC hdc)
{
	Super::Render(hdc);
}

void PopUp::SetVisible(bool visible)
{
	UI::SetVisible(visible);

	//if (visible == false)
	//	ResetPos();
}
