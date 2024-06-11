#include "pch.h"
#include "StatusPanel.h"
#include "HP.h"
#include "MP.h"
#include "Gold.h"
#include "ArrowUI.h"
#include "Potion.h"

StatusPanel::StatusPanel()
{
}

StatusPanel::~StatusPanel()
{
}

void StatusPanel::BeginPlay()
{
	{	// HP
		auto hp = make_shared<HP>();
		hp->SetVisible(true);
		AddChild(hp);
	}
	{  // MP
		auto mp = make_shared<MP>();
		mp->SetVisible(true);
		AddChild(mp);
	}

	{	// Gold
		auto gold = make_shared<Gold>();
		gold->SetVisible(true);
		AddChild(gold);
	}
	{	// Arrow UI
		auto arrowUI = make_shared<ArrowUI>();
		arrowUI->SetVisible(true);
		AddChild(arrowUI);
	}
	{	// Potion UI
		auto potion = make_shared<Potion>();
		potion->SetVisible(true);
		AddChild(potion);
	}

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
