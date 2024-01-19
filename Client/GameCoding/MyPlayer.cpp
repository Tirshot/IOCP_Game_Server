#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "ResourceManager.h"
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


MyPlayer::MyPlayer()
{
	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);

	_plum = GET_SINGLE(ResourceManager)->GetTexture(L"Crown");
}

MyPlayer::~MyPlayer()
{

}

void MyPlayer::BeginPlay()
{
	Super::BeginPlay();
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
	if (_potionNums <= 0)
		return;

	if (info.maxhp() == info.hp())
		return;

	info.set_hp(clamp(info.hp() + 1, 0, info.maxhp()));
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	//
	clamp(_potionNums -= 1, 0, 99);
}

void MyPlayer::AddPotion(int num)
{
	clamp(_potionNums += num, 0, 99); 
}

void MyPlayer::TickInput()
{
	_keyPressed = true;

	float _deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += _deltaTime;

	// deltaXY = {위, 아래, 왼쪽, 오른쪽}
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	// 채팅 입력창
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::Enter))
	{	
		SetState(CHAT); // _chatState는 SetVisible 안에서 수정됨
	}
	
	// 공격
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		NPC* npc = dynamic_cast<NPC*>(scene->GetCreatureAt(GetFrontCellPos()));

		if (npc)
			return;

		SetState(SKILL);
		_sumTime = 0;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::R))
	{
		UsePotion();
		scene->SpawnObject<HealEffect>({ GetCellPos() });
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar)
		&& GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
	{
		if (_sumTime >= 1.5f && GetState() != Protocol::OBJECT_STATE_TYPE_SPIN)
		{
			NPC* npc = dynamic_cast<NPC*>(scene->GetCreatureAt(GetFrontCellPos()));

			if (npc)
				return;

			SetState(SPIN_READY);
			_sumTime = 0;
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

	// 무기 선택, UI 연동 필요
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_SWORD);
		_selectedSlot = 0;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_BOW);
		_selectedSlot = 1;
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_STAFF);
		_selectedSlot = 2;
	}

	// Debug - 상인 앞으로 위치 이동
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_7))
	{
		info.set_gold(info.gold() + 1000);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		GET_SINGLE(ChatManager)->AddMessage(L"보유 골드 증가");
	}

	// Debug - 상인 앞으로 위치 이동
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_8))
	{
		SetCellPos({ 40,26 }, true);
		GET_SINGLE(ChatManager)->AddMessage(L"상인 앞으로 이동");
	}

	// Debug - CellPos 확인
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_9))
	{
		Vec2Int CellPos = GetCellPos();
		GET_SINGLE(ChatManager)->AddMessage(format(L"위치 : ({0}, {1})", CellPos.x, CellPos.y));
	}

	// Debug - 사망
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_0))
	{
		info.set_hp(0);
		Monster* monster = new Monster();
		OnDamaged(monster);
		{
			SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
			GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
		}
		delete monster;
	}
}

void MyPlayer::TryMove()
{
	if (_keyPressed == false)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	if (info.dir() == DIR_UP)
	{
		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_DOWN)
	{
		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_LEFT)
	{
		// 다음 칸으로 이동하기
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_RIGHT)
	{
		// 다음 칸으로 이동하기
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
	/*Super::TickTeleport();*/
	if (info.mp() >= 25)
	{
		info.set_mp(clamp(info.mp() - 25, 0, info.maxmp()));
	}
	else
	{
		GET_SINGLE(ChatManager)->AddMessage(L"마나가 부족합니다.");
	}
	SetState(IDLE);
}
void MyPlayer::SyncToServer()
{
	// 매 1000프레임마다 동기화하긴 불합리
	// 언제 보낼지 잘 판단하는 것이 중요

	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}