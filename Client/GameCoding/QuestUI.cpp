#include "pch.h"
#include "QuestUI.h"
#include "MerchantUI.h"
#include "QuestUIPanel.h"
#include "Texture.h"
#include "Button.h"
#include "TextBox.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

QuestUI::QuestUI()
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopUIBackground");

	{ // 뒤로 버튼
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + 585, _pos.y + 435 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &QuestUI::OnClickBackButton);
		AddChild(back);
	}

	{ // 대화 종료
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ _pos.x + 585, _pos.y + 485 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &QuestUI::OnClickExitButton);
		AddChild(exit);
	}
}

QuestUI::~QuestUI()
{
}

void QuestUI::BeginPlay()
{	
	for (auto& child : _children)
		child->BeginPlay();
}

void QuestUI::Tick()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	for (auto& child : _children)
		child->Tick();

	if (GetVisible() == true)
	{
		DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

		if (scene)
			_quests = scene->GetQuests();
	}
}

void QuestUI::Render(HDC hdc)
{
	::BitBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_background->GetDC(),
		0,
		0,
		SRCCOPY);

	for (auto& child : _children)
		child->Render(hdc);
}

void QuestUI::OnClickBackButton()
{
	SetVisible(false);

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);
}

void QuestUI::OnClickExitButton()
{
	SetVisible(false);
}


void QuestUI::ResetQuestList()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		_quests = scene->GetQuests();
	}
	int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
	
	for (auto& child : _children)
	{
		auto panel = FindChild<QuestUIPanel>(_children);
		if (panel)
			RemoveChild(panel);
	}

	_idx = 1;

	for (auto& questInfo : _quests)
	{
		// 아직 수주받지 않은 퀘스트를 받아옴
		auto state = scene->GetPlayerQuestState(myPlayerId, questInfo.first);
		if (state == Protocol::QUEST_STATE_FINISHED)
			continue;

		QuestUIPanel* questPanel = new QuestUIPanel(questInfo.second, _idx);
		questPanel->SetPos({ _pos.x + 5, (_pos.y + 20) * (float)_idx });
		AddChild(questPanel);
		_idx++;
	}
}

