#include "pch.h"
#include "QuestUI.h"
#include "MerchantUI.h"
#include "QuestUIPanel.h"
#include "Texture.h"
#include "Button.h"
#include "TextBox.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "MerchantDialogueUI.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "QuestManager.h"

QuestUI::QuestUI()
{
}

QuestUI::~QuestUI()
{
}

void QuestUI::BeginPlay()
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"ShopUIBackground");
	_rect = {};
	// _pos = 135, 80
	{
		_rect.left = _pos.x;
		_rect.top = _pos.y;
		_rect.right = _pos.x + 535;
		_rect.bottom = _pos.y + 450;
	}

	{ // �ڷ� ��ư
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + 450, _pos.y + 360 });
		back->SetSize({ 140, 40 });
		back->AddOnClickDelegate(this, &QuestUI::OnClickBackButton);
		back->SetInitialPos(back->GetPos());
		AddChild(back);
	}

	{ // ��ȭ ����
		Button* exit = new Button();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ _pos.x + 450, _pos.y + 410 });
		exit->SetSize({ 140, 40 });
		exit->AddOnClickDelegate(this, &QuestUI::OnClickExitButton);
		exit->SetInitialPos(exit->GetPos());
		AddChild(exit);
	}
	// ������ ����
	{
		Button* minus = new Button();
		minus->SetPos({ _pos.x + 400, _pos.y + 300 });
		minus->SetSize({ 40,24 });
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Default);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Hovered);
		minus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"LButton"), ButtonState::BS_Clicked);
		minus->AddOnClickDelegate(this, &QuestUI::OnClickCountMinusButton);
		minus->SetInitialPos(minus->GetPos());
		AddChild(minus);
	}

	// ������ ����
	{
		Button* plus = new Button();
		plus->SetPos({ _pos.x + 500, _pos.y + 300 });
		plus->SetSize({ 40,24 });
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Default);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Hovered);
		plus->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"RButton"), ButtonState::BS_Clicked);
		plus->AddOnClickDelegate(this, &QuestUI::OnClickCountPlusButton);
		plus->SetInitialPos(plus->GetPos());
		AddChild(plus);
	}
	// ����Ʈ ����
	{
		TextBox* textBox = new TextBox();
		textBox->SetText(L"����Ʈ�� �����ϼ���.");
		textBox->SetSize(Vec2Int{ 160 , 250 });
		textBox->SetPadding(5, 5);
		textBox->SetVisible(false);
		textBox->SetPos(Vec2{ _pos.x + 368, _pos.y + 5 });
		textBox->SetInitialPos(textBox->GetPos());
		AddChild(textBox);
	}

	_initialPos = _pos;

	for (auto& child : _children)
		child->BeginPlay();
}

void QuestUI::Tick()
{
	Panel::DragAndMove(&_rect);
	{
		_rect.left = _pos.x;
		_rect.top = _pos.y;
		_rect.right = _pos.x + 535;
		_rect.bottom = _pos.y + 450;
	}

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	for (auto& child : _children)
		child->Tick();

	if (GetVisible() == false)
		return;

	if (scene)
		_quests = scene->GetQuests();
	else
		return;

	for (auto& child : _children)
	{
		auto Panel = dynamic_cast<QuestUIPanel*>(child);
		// ����Ʈ �г��� Ŭ�� �� ��ȭ �ߵ�
		if (Panel && Panel->GetVisible() && IsMouseInRect(Panel->GetRect()))
		{
			int questID = Panel->GetQuestId();

			if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
			{
				auto DialoguePanel = scene->FindUI<MerchantDialogueUI>(scene->GetUIs());
				if (DialoguePanel)
				{
					DialoguePanel->SetDialogue(questID);
					DialoguePanel->SetVisible(true);
					//DialoguePanel->SetRewardItem();
					this->SetVisible(false);
					break;
				}
			}
		}
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

	// ������ ����
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		wstring Pages = to_wstring(_page) + L" / " + to_wstring(1 + (int)(_quests.size() / 4));
		RECT _textRect = { _pos.x + 435, _pos.y + 290,_textRect.left + 30,_textRect.top + 18 };
		DrawTextW(hdc, Pages.c_str(), -1, &_textRect, DT_CENTER);
	}

	for (auto& child : _children)
	{
		auto* Item = dynamic_cast<QuestUIPanel*>(child);
		if (Item)
		{
			int index = Item->GetIndex();
			int page = 1 + index / 4;

			// �����ִ� �������� �������� �ִ� �������� �������� ������ �ǳʶ�
			if (_page != page)
				continue;
		}
		// �������� �����ϰų� QuestUIPanel�� �ƴϸ� ������
		child->Render(hdc);
	}
}

void QuestUI::OnClickBackButton()
{
	SetVisible(false);
	ResetPage();
	ResetPos();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	MerchantUI* merchantUI = scene->FindUI<MerchantUI>(scene->GetUIs());
	merchantUI->SetVisible(true);
}

void QuestUI::OnClickExitButton()
{
	SetVisible(false);
	ResetPage();
	ResetPos();
}

void QuestUI::OnClickCountMinusButton()
{
	_page--;
	_page = ::clamp(_page, 1, 1 + ((int)_quests.size() / 4));
}

void QuestUI::OnClickCountPlusButton()
{
	_page++;
	_page = ::clamp(_page, 1, 1 + ((int)_quests.size() / 4));
}


void QuestUI::ResetQuestList()
{
	_idx = 0;

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		_quests = scene->GetQuests();

	int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
	
	for (auto& child : _children)
	{
		auto panel = FindChild<QuestUIPanel>(_children);
		if (panel)
			RemoveChild(panel);
	}

	for (auto& questInfo : _quests)
	{
		// ���� ���ֹ��� ���� ����Ʈ�� �޾ƿ�
		auto state = scene->GetPlayerQuestState(myPlayerId, questInfo.first);
		if (state == Protocol::QUEST_STATE_FINISHED)
			continue;

		SetQuestPanel(questInfo);
		_idx++;
	}
}

void QuestUI::SetQuestPanel(pair<int, Protocol::QuestInfo> questInfo)
{
	QuestUIPanel* questPanel = new QuestUIPanel(questInfo.second, _pos, _idx);
	AddChild(questPanel);
}

