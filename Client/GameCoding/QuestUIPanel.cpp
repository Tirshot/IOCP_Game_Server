#include "pch.h"
#include "QuestUIPanel.h"
#include "Texture.h"
#include "Sprite.h"
#include "MyPlayer.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "DevScene.h"
#include "MerchantUI.h"
#include "ShopUI.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "ChatManager.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "ClientPacketHandler.h"

QuestUIPanel::QuestUIPanel()
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"QuestButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	SetPos({135,80});
	int _questId = 0;
	wstring _questName = L"기본 퀘스트 이름";
	wstring _description = L"기본 퀘스트의 설명입니다. \n OO 몬스터를 몇마리 잡아오세요.";
	Protocol::OBJECT_TYPE _objectType = Protocol::OBJECT_TYPE_MONSTER;
	int _targetNums = 10;
	Protocol::QUEST_STATE _questState = Protocol::QUEST_STATE_IDLE;
}

QuestUIPanel::QuestUIPanel(Protocol::QuestInfo& info, Vec2 pos, int idx)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"QuestButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	_index = idx;

	SetSize({ 360,100 });
	SetPos({ 135,150 });

	_questId = info.questid();
	_targetId = info.targetid();
	_objectType = info.targettype();
	_targetNums = info.targetnums();
	_questState = info.queststate();
	_reward = info.rewardgold();
	_rewardItem = info.rewarditem();
	_rewardItemNum = info.rewarditemnum();

	int page = 1 + _index / 3;

	SetPos(Vec2{ pos.x + 5 , pos.y + 5 + GetSize().y * (_index % 4) });

	_rect = RECT{ (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y };
	_initialPos = _pos;

	// CSV파일을 이용해서 퀘스트 목록을 불러옴 
	auto questInfo = GET_SINGLE(QuestManager)->GetQuestInfo(_questId);

	_questName = questInfo.questName;
	_questNPC = questInfo.questNPC;
	_description = questInfo.description;

	{ // 퀘스트 수락 버튼
		Button* accept = new Button();
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"AcceptButton"), BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"CompleteButton"), BS_Pressed);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"AcceptButton"), BS_Hovered);
		accept->SetPos({ _pos.x + 41, _pos.y + 61 + (_size.y) * (float)_index });
		accept->SetSize({ 32, 21 });
		accept->AddOnClickDelegate(this, &QuestUIPanel::OnClickAcceptButton);
		_accept = accept;
		AddChild(_accept);
	}

	{ // 퀘스트 완료 버튼
		Button* complete = new Button();
		complete->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"CompleteButton"), BS_Default);
		complete->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"CompleteButton"), BS_Pressed);
		complete->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"CompleteButton"), BS_Hovered);
		complete->SetPos({ _pos.x + 41, _pos.y + 61 + (_size.y) * (float)_index });
		complete->SetSize({ 32, 21 });
		complete->AddOnClickDelegate(this, &QuestUIPanel::OnClickCompleteButton);
		complete->SetVisible(false);
		_complete = complete;
		AddChild(_complete);
	}
}

QuestUIPanel::~QuestUIPanel()
{
}

void QuestUIPanel::BeginPlay()
{
	for (auto& child : _children)
		child->BeginPlay();
}

void QuestUIPanel::Tick()
{
	_rect = { (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y };

	for (auto& child : _children)
		child->Tick();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();

	_questState = scene->GetPlayerQuestState(myPlayerId, _questId);

	if (_questState == Protocol::QUEST_STATE_ACCEPT)
	{
		RemoveChild(_accept);
		_complete->SetVisible(true);
	}
}

void QuestUIPanel::Render(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	// Button Background
	::BitBlt(hdc,
		_pos.x,
		_pos.y,
		_size.x,
		_size.y,
		_background->GetDC(),
		0,
		0,
		SRCCOPY);

	// 퀘스트 이름
	{
		RECT _textRect = { _pos.x + 70,_pos.y + 10, _textRect.left + 250, _textRect.top + 18 };
		DrawTextW(hdc, _questName.c_str(), -1, &_textRect, DT_LEFT);
	}
	// 퀘스트 설명
	{
		RECT _textRect = { _pos.x + 70,_pos.y + 30, _textRect.left + 290, _textRect.top + 68 };
		DrawTextW(hdc, _description.c_str(), -1, &_textRect, DT_LEFT | DT_WORDBREAK);
	}
	// 골드 이미지
	{
		::TransparentBlt(hdc,
			_pos.x + 300,
			_pos.y + 72,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetDC(),
			0,
			0,
			_goldImage->GetSize().x / 3,
			_goldImage->GetSize().y,
			_goldImage->GetTransparent());
	}
	// 보상 Gold
	{
		wstring wstr = to_wstring(_reward);
		RECT _textRect = { _pos.x + 325,_pos.y + 78, _textRect.left + 250, _textRect.top + 18 };
		DrawTextW(hdc, wstr.c_str(), -1, &_textRect, DT_LEFT);
	}

	for (auto& child : _children)
		child->Render(hdc);
}

void QuestUIPanel::OnClickAcceptButton()
{

}

void QuestUIPanel::OnClickCompleteButton()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
		auto state = scene->GetPlayerQuestState(myPlayerId, _questId);
		if (state == Protocol::QUEST_STATE_COMPLETED)
		{
			// 퀘스트 완료
			{
				MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

				if (myPlayer)
				{
					if (GET_SINGLE(ItemManager)->IsInventoryFull())
					{
						// 인벤토리가 가득 차(찰 예정이라)서 퀘스트 완료 불가 -> 추후 코드 수정필요
						return;
					}

					myPlayer->info.set_gold(myPlayer->info.gold() + _reward);
					GET_SINGLE(ItemManager)->AddItemToInventory(_rewardItem, _rewardItemNum);
					// _rewardItem?
				}
				_questState = Protocol::QUEST_STATE_FINISHED;
				scene->SetPlayerQuestState(myPlayerId, _questId, Protocol::QUEST_STATE_FINISHED);
				RemoveChild(_complete);
				RemoveChild(_accept);
				GET_SINGLE(SoundManager)->Play(L"QuestFinished");
			}
		}
	}
}
