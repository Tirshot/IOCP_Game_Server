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
	_isLinkQuest = info.islinkquest();
	_prevQuestID = info.prevquestid();

	int page = 1 + _index / 3;

	SetPos(Vec2{ pos.x + 5 , pos.y + 5 + GetSize().y * (_index % 4) });

	_rect = RECT{ (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y };
	_initialPos = _pos;

	// CSV파일을 이용해서 퀘스트 목록을 불러옴 
	auto questInfo = GET_SINGLE(QuestManager)->GetQuestScriptInfo(_questId);

	_questName = questInfo.questName;
	_questNPC = questInfo.questNPC;
	_description = questInfo.description;
}

QuestUIPanel::~QuestUIPanel()
{
	_background = nullptr;
	_questImage = nullptr;
	_goldImage = nullptr;
	_accept = nullptr;
	_complete = nullptr;
}

void QuestUIPanel::BeginPlay()
{
	Super::BeginPlay();
}

void QuestUIPanel::Tick()
{
	Super::Tick();

	_rect = { (int)_pos.x, (int)_pos.y, (int)_pos.x + GetSize().x, (int)_pos.y + GetSize().y };

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();

	_questState = scene->GetPlayerQuestState(myPlayerId, _questId);
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
	if (_reward != 0)
	{
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
	}
	// 보상 Gold
	if (_reward != 0)
	{
		wstring wstr = to_wstring(_reward);
		RECT _textRect = { _pos.x + 325,_pos.y + 78, _textRect.left + 250, _textRect.top + 18 };
		DrawTextW(hdc, wstr.c_str(), -1, &_textRect, DT_LEFT);
	}

	Super::Render(hdc);
}