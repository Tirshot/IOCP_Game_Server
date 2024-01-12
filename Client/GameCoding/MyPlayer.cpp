#include "pch.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
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
}

uint64 prevCount = 0;
uint64 nowCount = 0;
void MyPlayer::TickInput()
{
	_keyPressed = true;
	nowCount = GetTickCount64();

	// deltaXY = {��, �Ʒ�, ����, ������}
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	// ä�� �Է�â
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::Enter))
	{	
		SetState(CHAT); // _chatState�� SetVisible �ȿ��� ������
	}
	
	// ����
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		SetState(SKILL);
		prevCount = nowCount;
	}

	else if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar)
		&& GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
	{
		if (nowCount - prevCount >= 1500.f && GetState() != Protocol::OBJECT_STATE_TYPE_SPIN)
		{
			SetState(SPIN_READY);
			prevCount = nowCount;
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

	// ���� ����, UI ���� �ʿ�
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_SWORD);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_BOW);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3))
	{
		// ���Ŀ� ������ ���濹��
		SetWeaponType(Protocol::WEAPON_TYPE_STAFF);
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
void MyPlayer::SyncToServer()
{
	// �� 1000�����Ӹ��� ����ȭ�ϱ� ���ո�
	// ���� ������ �� �Ǵ��ϴ� ���� �߿�

	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}