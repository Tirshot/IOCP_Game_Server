#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "Flipbook.h"
#include "Texture.h"
#include "NPC.h"
#include "Monster.h"
#include "TimeManager.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "HealEffect.h"
#include "TeleportEffect.h"
#include "Arrow.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"
#include "ChatManager.h"
#include "Chat.h"
#include "GameOver.h"
#include "Inventory.h"
#include "PopUp.h"
#include "ItemManager.h"


MyPlayer::MyPlayer()
{
	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);
}

MyPlayer::~MyPlayer()
{

}

void MyPlayer::BeginPlay()
{
	Super::BeginPlay();

	_plum = GET_SINGLE(ResourceManager)->GetTexture(L"Crown");
}

void MyPlayer::Tick()
{
	Super::Tick();

	SyncToServer();
}

void MyPlayer::Render(HDC hdc)
{
	Super::Render(hdc);

	// Plum
	if (_plum == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	::TransparentBlt(hdc,
		(int32)_pos.x - 10 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - 70 - ((int32)cameraPos.y - GWinSizeY / 2),
		21,
		20,
		_plum->GetDC(),
		0,
		0,
		21,
		20,
		_plum->GetTransparent());
}

void MyPlayer::Teleport(Vec2Int cellPos)
{
	SetCellPos(cellPos, true);

	SyncToServer();
}

int MyPlayer::GetSelectedSlot()
{
	return _selectedSlot;
}

void MyPlayer::UsePotion()
{
	_potionNums = info.potion();

	if (_potionNums <= 0)
		return;

	if (info.maxhp() == info.hp())
		return;

	auto potionEffectMultiply = GET_SINGLE(ItemManager)->GetPotionEffectMultiply();

	// Values.h ���� ��ġ ���� ����
	info.set_hp(clamp(info.hp() + POTION_HEALING_AMOUNT * potionEffectMultiply, 0, info.maxhp()));

	GET_SINGLE(SoundManager)->Play(L"Potion");
	info.set_potion(clamp(info.potion() - 1, 0, 99));
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Heal(info.objectid());
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	scene->SpawnObject<HealEffect>({ GetCellPos() });
}

void MyPlayer::TickInput()
{
	_keyPressed = true;

	float _deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTimes += _deltaTime;

	// deltaXY = {��, �Ʒ�, ����, ������}
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	auto scene = GET_SINGLE(SceneManager)->GetDevScene();
	// ä�� �Է�â
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::Enter))
	{	
		SetState(CHAT); // _chatState�� SetVisible �ȿ��� ������
	}
	
	// ����
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		auto npc = dynamic_pointer_cast<NPC>(scene->GetCreatureAt(GetFrontCellPos()));

		if (npc)
			return;

		SetState(SKILL);
		_sumTimes = 0;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::R))
	{
		UsePotion();
	}

	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::I))
	{
		GET_SINGLE(ItemManager)->OpenInventory();

		//  �� ������ ������ ����
		GET_SINGLE(ItemManager)->GetInventory()->AutoSyncInventory();
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar)
		&& GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
	{
		if (_sumTimes >= 0.5f && GetState() != Protocol::OBJECT_STATE_TYPE_SPIN)
		{
			auto npc = dynamic_pointer_cast<NPC>(scene->GetCreatureAt(GetFrontCellPos()));

			if (npc)
				return;

			SetState(SPIN_READY);
			_sumTimes = 0;
		}
	}

	 if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
	{
		SetDir(DIR_UP);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
	{
		SetDir(DIR_DOWN);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::A))
	{
		SetDir(DIR_LEFT);
	}
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::D))
	{
		SetDir(DIR_RIGHT);
	}
	else
		_keyPressed = false;

	// Debug - ���� ȭ�� ���� ����
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Z))
	{
		info.set_arrows(info.arrows() + 10);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		GET_SINGLE(ChatManager)->AddMessage(L"DEBUG : ���� ȭ�� ����");
		GET_SINGLE(ChatManager)->SendMessageToServer(L"DEBUG : ���� ȭ�� ����", false);
	}

	// Debug - ���� ��� ����
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::X))
	{
		info.set_gold(info.gold() + 1000);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		GET_SINGLE(ChatManager)->AddMessage(L"DEBUG : ���� ��� ����");
		GET_SINGLE(ChatManager)->SendMessageToServer(L"DEBUG : ���� ��� ����", false);
	}

	// Debug -	Move
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::C))
	{
		SetCellPos({ 44,25 }, true);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		GET_SINGLE(ChatManager)->AddMessage(L"DEBUG : ���ο��� �����̵�");
		GET_SINGLE(ChatManager)->SendMessageToServer(L"DEBUG : ���ο��� �����̵�", false);
	}

	// Debug - CellPos Ȯ��
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::V))
	{
		Vec2Int CellPos = GetCellPos();
		GET_SINGLE(ChatManager)->AddMessage(format(L"��ġ : ({0}, {1})", CellPos.x, CellPos.y));
	}

	// Debug - ���� ��ġ�� ���� ų
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::B))
	{
		auto monster = scene->GetMonster();
		auto creature = dynamic_pointer_cast<Creature>(monster);

		if (monster)
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Hit(monster->info.objectid(), info.objectid());
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);

			GET_SINGLE(ChatManager)->AddMessage(format(L"DEBUG : ({0}, {1}) ��ġ�� ���͸� óġ.", monster->GetCellPos().x, monster->GetCellPos().y));
			GET_SINGLE(ChatManager)->SendMessageToServer(format(L"DEBUG : ({0}, {1}) ��ġ�� ���͸� óġ.", monster->GetCellPos().x, monster->GetCellPos().y), false);
		}
	}

	// Debug - ���
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::M))
	{
		//info.set_hp(0);

		GET_SINGLE(ChatManager)->AddMessage(L"DEBUG : ���");
		GET_SINGLE(ChatManager)->SendMessageToServer(L"DEBUG : ���", false);

		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_KillPlayer(GetObjectID());
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
	}
}

void MyPlayer::TryMove()
{
	if (_keyPressed == false)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	if (info.dir() == DIR_UP)
	{
		// ���� ĭ���� �̵��ϱ�
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_DOWN)
	{
		// ���� ĭ���� �̵��ϱ�
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_LEFT)
	{
		// ���� ĭ���� �̵��ϱ�
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_RIGHT)
	{
		// ���� ĭ���� �̵��ϱ�
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
}

void MyPlayer::TickIdle()
{
	TickInput();
	TryMove();
}

void MyPlayer::TickMove()
{
	Super::TickMove();
}

void MyPlayer::TickSkill()
{
	Super::TickSkill();
}

void MyPlayer::TickHit()
{
	Super::TickHit();
}

void MyPlayer::TickChat()
{
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::Enter))
	{
		SetState(IDLE);
		GET_SINGLE(ChatManager)->GetChat()->SetChatState(false);
	}
}

void MyPlayer::TickSpin()
{
	Super::TickSpin();
}

void MyPlayer::TickSpinReady()
{
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::SpaceBar))
	{
		SetState(SPIN);
	}
}

void MyPlayer::TickTeleport()
{
	if (info.mp() >= 25)
	{
		info.set_mp(clamp(info.mp() - 25, 0, info.maxmp()));
	}
	else
	{
		GET_SINGLE(ChatManager)->AddMessage(L"������ �����մϴ�.");
	}
	SetState(IDLE);
}

void MyPlayer::SyncToServer()
{
	// �� 1000�����Ӹ��� ����ȭ�ϱ� ���ո�
	// ���� ������ �� �Ǵ��ϴ� ���� �߿�

	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}


int MyPlayer::GetQuestProgress(int questId)
{
	return _questsStates[questId].second;
}

map<int, pair<Protocol::QUEST_STATE, int>> MyPlayer::GetActiveQuests()
{
	map<int, pair<Protocol::QUEST_STATE, int>> activeQuests;

	for (auto& quest : _questsStates)
	{
		int questID = quest.first;
		auto state = quest.second.first;
		int questProgress = quest.second.second;

		if (state == Protocol::QUEST_STATE_ACCEPT ||
			state == Protocol::QUEST_STATE_COMPLETED)
		{
			activeQuests.insert(quest);
		}
	}

	return activeQuests;
}

void MyPlayer::SetQuestProgress(int questId, int progress)
{
	_questsStates[questId].second = progress;
}