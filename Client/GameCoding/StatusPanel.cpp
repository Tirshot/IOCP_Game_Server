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
	_rect = {};
}

void StatusPanel::BeginPlay()
{
	Super::BeginPlay();

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
}

void StatusPanel::Tick()
{
	Super::Tick();
}

void StatusPanel::Render(HDC hdc)
{
	Super::Render(hdc);
}
