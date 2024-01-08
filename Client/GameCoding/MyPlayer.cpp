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
uint64 spinStart = 0;
void MyPlayer::TickInput()
{
	_keyPressed = true;
	nowCount = GetTickCount64();

	// deltaXY = {위, 아래, 왼쪽, 오른쪽}
	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	// 채팅 입력창
	if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::Enter))
	{	
		SetState(CHAT); // _chatState는 SetVisible 안에서 수정됨
	}
	
	// 공격
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		// 500ms
		if (nowCount - prevCount >= 500.f)
		{
			SetState(SKILL);
			prevCount = nowCount;
			spinStart = GetTickCount64();
			_isSpinning = false;
		}
	}

	else if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar) && GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
	{
		// 경과 시간 측정
		float elapsedTime = static_cast<float>(GetTickCount64() - spinStart);

		// 1500ms 이상 경과 시간이 지나면 스핀 공격 시작
		if (elapsedTime >= 1500.f && _isSpinning == false)
		{
			_isSpinning = true;
			spinStart = GetTickCount64();
			SetState(IDLE);
			// 스핀 공격 시작에 대한 추가 처리
			// 예: 애니메이션 변경, 효과 발동 등
		}
		else if (_isSpinning)
		{
			_isSpinning = false;
			spinStart = GetTickCount64();
			SetState(SPIN_READY);
			// 빛나는 애니메이션 출력
			// 스페이스 바를 누르고 있는 동안의 일반 공격 처리
			// 예: 공격 애니메이션, 이동 등
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
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(Protocol::WEAPON_TYPE_BOW);
	}
	else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3))
	{
		// 추후에 갈고리로 변경예정
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

void MyPlayer::SyncToServer()
{
	// 매 1000프레임마다 동기화하긴 불합리
	// 언제 보낼지 잘 판단하는 것이 중요

	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}