#include "pch.h"
#include "MerchantDialogueUI.h"
#include "Sprite.h"
#include "Button.h"
#include "TextBox.h"
#include "NamePlate.h"
#include "DevScene.h"
#include "QuestUI.h"
#include "MyPlayer.h"
#include "ResourceManager.h"
#include "QuestManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ItemManager.h"
#include "NetworkManager.h"
#include "SoundManager.h"
#include "ClientPacketHandler.h"

MerchantDialogueUI::MerchantDialogueUI()
{
}

MerchantDialogueUI::~MerchantDialogueUI()
{
}

void MerchantDialogueUI::BeginPlay()
{
	_rect = {135, 340, 670, 520};
	_size = { 535, 180 };
	_pos = { 135, 340 };
	_initialPos = _pos;
	
	{ // 2D 스프라이트
		_merchantSprite = GET_SINGLE(ResourceManager)->GetSprite(L"MerchantSprite");
		_backGround = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");
	}
	{ // 대화 내용
		wstring wstr = L"퀘스트 대화 내용입니다.";
		TextBox* textBox = new TextBox(wstr);
		textBox->SetPos({ _pos.x, _pos.y });
		textBox->SetPadding( 15, 15 );
		textBox->SetSize({ _size.x, _size.y });
		AddChild(textBox);
	}

	{ // 네임 플레이트
		NamePlate* namePlate = new NamePlate(L"언덕 집 상인");
		namePlate->SetPos(Vec2{ _pos.x + 65, _pos.y - 40 });
		AddChild(namePlate);
	}

	{ // 퀘스트 수락
		Button* accept = new Button();
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Accept_Off"), BS_Default);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Accept_On"), BS_Pressed);
		accept->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Accept_Hovered"), BS_Hovered);
		accept->SetPos(Vec2{ _pos.x + (_size.x / 2) - 70, _pos.y + (_size.y) - 30 });
		accept->SetSize({ 105, 30 });
		accept->AddOnClickDelegate(this, &MerchantDialogueUI::OnClickAcceptButton);
		accept->SetInitialPos(accept->GetPos());
		accept->SetVisible(false);
		accept->SetButtonName(L"accept");
		AddChild(accept);
	}

	{ // 퀘스트 거절
		Button* decline = new Button();
		decline->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Decline_Off"), BS_Default);
		decline->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Decline_On"), BS_Pressed);
		decline->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Quest_Decline_Hovered"), BS_Hovered);
		decline->SetPos(Vec2{ _pos.x + (_size.x / 2) + 70, _pos.y + (_size.y) - 30 });
		decline->SetSize({ 105, 30 });
		decline->AddOnClickDelegate(this, &MerchantDialogueUI::OnClickDeclineButton);
		decline->SetInitialPos(decline->GetPos());
		decline->SetVisible(false);
		decline->SetButtonName(L"decline");
		AddChild(decline);
	}

	{ // 확인 버튼
		Button* confirm = new Button();
		confirm->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Confirm_Off"), BS_Default);
		confirm->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Confirm_On"), BS_Pressed);
		confirm->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Confirm_Hovered"), BS_Hovered);
		confirm->SetPos(Vec2{ _pos.x + _size.x - 80, _pos.y + (_size.y) - 30 });
		confirm->SetSize({ 105, 30 });
		confirm->AddOnClickDelegate(this, &MerchantDialogueUI::OnClickConfirmButton);
		confirm->SetInitialPos(confirm->GetPos());
		confirm->SetVisible(false);
		confirm->SetButtonName(L"confirm");
		AddChild(confirm);
	}

	{ // 뒤로 버튼
		Button* back = new Button();
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Off"), BS_Default);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_On"), BS_Pressed);
		back->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Back_Hovered"), BS_Hovered);
		back->SetPos(Vec2{ _pos.x + _size.x - 80, _pos.y + (_size.y) - 30});
		back->SetSize({ 105, 30 });
		back->AddOnClickDelegate(this, &MerchantDialogueUI::OnClickDeclineButton);
		back->SetInitialPos(back->GetPos());
		back->SetVisible(false);
		back->SetButtonName(L"back");
		AddChild(back);
	}

	for (auto& child : _children)
		child->BeginPlay();
}

void MerchantDialogueUI::Tick()
{
	if (_scripts.size() == 0)
		return;

	_maxPage = _scripts.size();

	// 다이얼로그 진행 로직
	for (auto& child : _children)
	{
		auto textBox = dynamic_cast<TextBox*>(child);

		if (textBox)
		{
			textBox->SetText(_scripts[_page]);
			continue;
		}

		// 스크립트의 마지막 페이지에 도달
		// 수락, 거절 버튼 활성화
		if (_page == _maxPage - 1)
		{
			auto scene = GET_SINGLE(SceneManager)->GetDevScene();
			int myPlayerID = GET_SINGLE(SceneManager)->GetMyPlayerId();
			if (scene && myPlayerID)
			{
				_questState = scene->GetPlayerQuestState(myPlayerID, _questID);
				VisibleButton();
			}
		}
		else
		{
			auto button = dynamic_cast<Button*>(child);
			if (button)
			{
				button->SetVisible(false);
				continue;
			}
		}
	}

	if (IsMouseInRect(_rect))
	{
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse)||
			GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		{
			// 단문일 경우
			if (_maxPage == 1)
			{
				_page == _maxPage;
			}
			else
			{
				_page++;
				_page = ::clamp(_page, 1, _maxPage - 1);
			}
		}
	}

	// 퀘스트 상태 확인 - 완료시 확인 버튼 활성화

	for (auto& child : _children)
		child->Tick();
}

void MerchantDialogueUI::Render(HDC hdc)
{
	::TransparentBlt(hdc,
		_pos.x - 5,
		_pos.y - 290,
		_merchantSprite->GetSize().x * 3,
		_merchantSprite->GetSize().y * 3,
		_merchantSprite->GetDC(),
		0,
		0,
		_merchantSprite->GetSize().x,
		_merchantSprite->GetSize().y,
		_merchantSprite->GetTransparent());

	::StretchBlt(hdc,
		135,
		342,
		535,
		178,
		_backGround->GetDC(),
		50,
		50,
		100,
		100,
		SRCCOPY);

	for (auto& child : _children)
		child->Render(hdc);
}

void MerchantDialogueUI::SetDialogue(int questID)
{
	_questID = questID;
	_scripts = GET_SINGLE(QuestManager)->GetQuestScript(questID);
}

void MerchantDialogueUI::SetRewardItem(int itemID, int num)
{
	_rewardItem = itemID;
	_rewardItemNum = num;
}

void MerchantDialogueUI::SetRewardGold(int gold)
{
	_rewardGold = gold;
}

void MerchantDialogueUI::VisibleButton()
{
	// 버튼 활성화
	for (auto& child : _children)
	{
		// 처음 수령하는 퀘스트인지 확인
		auto button = dynamic_cast<Button*>(child);
		if (button)
		{
			switch (_questState)
			{
			case Protocol::QUEST_STATE_IDLE:
			{
				// 퀘스트 수락, 거절 버튼만 활성화
				if (button->GetButtonName() == L"accept" || button->GetButtonName() == L"decline")
				{
					button->SetVisible(true);
				}
				else
				{
					button->SetVisible(false);
				}
				continue;
			}
			case Protocol::QUEST_STATE_ACCEPT:
			{
				// 뒤로 버튼 활성화
				if (button->GetButtonName() == L"back")
				{
					button->SetVisible(true);
				}
				else
				{
					button->SetVisible(false);
				}
				continue;
			}
			case Protocol::QUEST_STATE_COMPLETED:
			{
				// 확인 버튼 활성화
				if (button->GetButtonName() == L"confirm")
				{
					button->SetVisible(true);
				}
				else
				{
					button->SetVisible(false);
				}
				continue;
			}
			default:
			{
				button->SetVisible(false);
				continue;
			}
			}
		}
	}
}

void MerchantDialogueUI::OnClickAcceptButton()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
		auto state = scene->GetPlayerQuestState(myPlayerId, _questID);
		if (state != Protocol::QUEST_STATE_IDLE)
			return;

		// 퀘스트 수락 패킷 전송
		{
			_questState = Protocol::QUEST_STATE_ACCEPT;

			scene->SetPlayerQuestState(myPlayerId, _questID, Protocol::QUEST_STATE_ACCEPT);
			{
				SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Quest(myPlayerId, _questID);
				GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			}
			GET_SINGLE(SoundManager)->Play(L"QuestAccept");
		}
	}

	// UI 감추기
	SetVisible(false);
	auto* questUI = scene->FindUI<QuestUI>(scene->GetUIs());

	// 다이얼로그 페이지 초기화
	ResetPage();

	if (questUI)
	{
		questUI->SetVisible(true);
		questUI->ResetPos();
		questUI->ResetPage();
	}
}

void MerchantDialogueUI::OnClickDeclineButton()
{
	// UI 감추기
	SetVisible(false);

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene == nullptr)
		return;

	auto* questUI = scene->FindUI<QuestUI>(scene->GetUIs());

	// 다이얼로그 페이지 초기화
	ResetPage();

	if (questUI)
	{
		questUI->SetVisible(true);
		questUI->ResetPos();
		questUI->ResetPage();
	}
}

void MerchantDialogueUI::OnClickConfirmButton()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		int myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
		auto state = scene->GetPlayerQuestState(myPlayerId, _questID);
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

					myPlayer->info.set_gold(myPlayer->info.gold() + _rewardGold);
					
					if (_rewardItem != 0)
						GET_SINGLE(ItemManager)->AddItemToInventory(_rewardItem, _rewardItemNum);
				}
				_questState = Protocol::QUEST_STATE_FINISHED;
				scene->SetPlayerQuestState(myPlayerId, _questID, Protocol::QUEST_STATE_FINISHED);
				GET_SINGLE(SoundManager)->Play(L"QuestFinished");
			}
		}
	}
}
