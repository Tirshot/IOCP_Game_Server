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

QuestUIPanel::QuestUIPanel(Protocol::QuestInfo& info, int idx)
{
	_background = GET_SINGLE(ResourceManager)->GetTexture(L"QuestButtonsBackground");
	_goldImage = GET_SINGLE(ResourceManager)->GetTexture(L"Gold");
	SetSize({ 360,100 });
	SetPos({ 135,150 });

	_questId = info.questid();
	_objectType = info.targettype();
	_targetNums = info.targetnums();
	_questState = info.queststate();
	_reward = info.rewardgold();
	_rewardItem = info.rewarditem();
	_rewardItemNum = info.rewarditemnum();

	switch (_questId)
	{
	case 0:
		_questName = L"뱀이 너무 많아!";
		_description = L"호기롭게 장사를 시작했지만, 손님이 적은 이유는 역시... 뱀 때문이겠죠?\n뱀 5마리만 처치해주세요.\n사례는 넉넉히 드리겠습니다.";
		break;

	case 1:
		_questName = L"이상한 문양";
		_description = L"제 집 위에 보이는 문양을 보셨나요? 제가 이사할 때부터 있었는데..\n용사만 할 수 있는 액션을 하면 빛이 난다는 전설이 있었어요.";
		break;

	case 2:
		_questName = L"테스트용2";
		_description = L"테스트용 퀘스트2입니다.";
		break;

	default:
		_questName = L"";
		_description = L"";
		break;
	}

	{ // 퀘스트 수락 버튼
		Button* accept = new Button();
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"AcceptButton"), BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"CompleteButton"), BS_Pressed);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"AcceptButton"), BS_Hovered);
		accept->SetPos({ 176, 61 + (_size.y) * (float)idx });
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
		complete->SetPos({ 176, 61 + (_size.y) * (float)idx });
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
		RECT _textRect = { _pos.x + 70,_pos.y + 28, _textRect.left + 290, _textRect.top + 68 };
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
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
		auto state = scene->GetPlayerQuestState(myPlayerId, _questId);
		if (state != Protocol::QUEST_STATE_IDLE)
			return;

		// 퀘스트 수락 패킷 전송
		{
			_questState = Protocol::QUEST_STATE_ACCEPT;

			scene->SetPlayerQuestState(myPlayerId, _questId, Protocol::QUEST_STATE_ACCEPT);
			{
				SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Quest(myPlayerId, _questId);
				GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			}
			GET_SINGLE(SoundManager)->Play(L"QuestAccept");
		}
	}
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
				_questState = Protocol::QUEST_STATE_FINISHED;
				scene->SetPlayerQuestState(myPlayerId, _questId, Protocol::QUEST_STATE_FINISHED);
				RemoveChild(_complete);
				RemoveChild(_accept);

				MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
				myPlayer->info.set_gold(myPlayer->info.gold() + _reward);
				// _rewardItem?
				GET_SINGLE(SoundManager)->Play(L"QuestFinished");
			}
		}
	}
}
