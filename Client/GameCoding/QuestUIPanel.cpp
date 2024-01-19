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
	wstring _questName = L"�⺻ ����Ʈ �̸�";
	wstring _description = L"�⺻ ����Ʈ�� �����Դϴ�. \n OO ���͸� ��� ��ƿ�����.";
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
		_questName = L"���� �ʹ� ����!";
		_description = L"ȣ��Ӱ� ��縦 ����������, �մ��� ���� ������ ����... �� �����̰���?\n�� 5������ óġ���ּ���.\n��ʴ� �˳��� �帮�ڽ��ϴ�.";
		break;

	case 1:
		_questName = L"�̻��� ����";
		_description = L"�� �� ���� ���̴� ������ ���̳���? ���� �̻��� ������ �־��µ�..\n��縸 �� �� �ִ� �׼��� �ϸ� ���� ���ٴ� ������ �־����.";
		break;

	case 2:
		_questName = L"�׽�Ʈ��2";
		_description = L"�׽�Ʈ�� ����Ʈ2�Դϴ�.";
		break;

	default:
		_questName = L"";
		_description = L"";
		break;
	}

	{ // ����Ʈ ���� ��ư
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

	{ // ����Ʈ �Ϸ� ��ư
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

	// ����Ʈ �̸�
	{
		RECT _textRect = { _pos.x + 70,_pos.y + 10, _textRect.left + 250, _textRect.top + 18 };
		DrawTextW(hdc, _questName.c_str(), -1, &_textRect, DT_LEFT);
	}
	// ����Ʈ ����
	{
		RECT _textRect = { _pos.x + 70,_pos.y + 28, _textRect.left + 290, _textRect.top + 68 };
		DrawTextW(hdc, _description.c_str(), -1, &_textRect, DT_LEFT | DT_WORDBREAK);
	}
	// ��� �̹���
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
	// ���� Gold
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

		// ����Ʈ ���� ��Ŷ ����
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
			// ����Ʈ �Ϸ�
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
