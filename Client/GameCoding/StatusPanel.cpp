#include "pch.h"
#include "StatusPanel.h"
#include "HP.h"
#include "MP.h"
#include "Gold.h"
#include "ArrowUI.h"
#include "Potion.h"

StatusPanel::StatusPanel()
{
	{	// HP
		HP* hp = new HP();
		hp->SetVisible(true);
		AddChild(hp);
	}
	{  // MP
		MP* mp = new MP();
		mp->SetVisible(true);
		AddChild(mp);
	}

	{	// Gold
		Gold* gold = new Gold();
		gold->SetVisible(true);
		AddChild(gold);
	}
	{	// Arrow UI
		ArrowUI* arrowUI = new ArrowUI();
		arrowUI->SetVisible(true);
		AddChild(arrowUI);
	}
	{	// Potion UI
		Potion* potion = new Potion();
		potion->SetVisible(true);
		AddChild(potion);
	}
}

StatusPanel::~StatusPanel()
{
}

void StatusPanel::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void StatusPanel::Tick()
{
	for (auto& child : _children)
		child->Tick();
}

void StatusPanel::Render(HDC hdc)
{
	for (auto& child : _children)
		child->Render(hdc);
}
