#include "pch.h"
#include "Player.h"
#include "MyPlayer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Flipbook.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"
#include "TeleportEffect.h"
#include "Arrow.h"
#include "Monster.h"
#include "ChatManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

Player::Player()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_flipbookAttack[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackUp");
	_flipbookAttack[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackDown");
	_flipbookAttack[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackLeft");
	_flipbookAttack[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackRight");

	_flipbookBow[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowUp");
	_flipbookBow[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowDown");
	_flipbookBow[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowLeft");
	_flipbookBow[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowRight");

	_flipbookStaff[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffUp");
	_flipbookStaff[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffDown");
	_flipbookStaff[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffLeft");
	_flipbookStaff[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_StaffRight");

	_flipbookSpin[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinUp");
	_flipbookSpin[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinDown");
	_flipbookSpin[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinLeft");
	_flipbookSpin[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinRight");

	_flipbookSpinReady[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinReadyUp");
	_flipbookSpinReady[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinReadyDown");
	_flipbookSpinReady[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinReadyLeft");
	_flipbookSpinReady[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SpinReadyRight");

	//이젠 서버에서 받아옴
	//info.set_hp(100);
	//info.set_maxhp(100);
	//info.set_attack(15);
	//info.set_defence(0);
}

Player::~Player()
{

}

void Player::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void Player::Tick()
{
	Super::Tick();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::TickIdle()
{

}

void Player::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 1.f )
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 160 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 160 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 160 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 160 * deltaTime;
			break;
		}
	}
}


void Player::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (IsAnimationEnded())
	{
		// 공격 판정
		DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
		if (scene == nullptr)
			return;

		if (GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
		{
			// 내 앞에 있는 좌표
			Monster* monster = dynamic_cast<Monster*>(scene->GetCreatureAt(GetFrontCellPos()));

			if (monster)
			{
				// 몬스터에 피격 이펙트 출력
				scene->SpawnObject<HitEffect>(GetFrontCellPos());
				monster->OnDamaged(this);

				// 몬스터 피격 스프라이트 출력 및 스턴 시간
				if (monster->info.hp() <=0)
					return;

				monster->SetWait(50);
				monster->SetState(HIT);
				monster->KnockBack(this);
			}
		}
		else if (GetWeaponType() == Protocol::WEAPON_TYPE_BOW)
		{
			// 화살 생성 패킷 전송
			MyPlayer* myPlayer = dynamic_cast<MyPlayer*>(this);

			// 화살 버그 수정 - 패킷 생성시 플레이어 수 만큼 나감 주의!!
			if (myPlayer)
			{
				SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Fire(GetObjectID());
				GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
			}
		}
		else if (GetWeaponType() == Protocol::WEAPON_TYPE_STAFF)
		{
			SetState(TELEPORT);
			return;
		}
		SetState(IDLE);
	}
}

void Player::TickSpin()
{
	if (_flipbook == nullptr)
		return;

	if (IsAnimationEnded())
	{
		// 공격 판정
		DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
		if (scene == nullptr)
			return;

		if (info.weapontype() == Protocol::WEAPON_TYPE_SWORD)
		{
			// 내 십자에 있는 좌표
			Monster* monster = dynamic_cast<Monster*>(scene->GetCreatureAt(GetCellPos() + Vec2Int{ 0,-1 })); // up
			Monster* monster2 = dynamic_cast<Monster*>(scene->GetCreatureAt(GetCellPos() + Vec2Int{ 1,0 })); // right
			Monster* monster3 = dynamic_cast<Monster*>(scene->GetCreatureAt(GetCellPos() + Vec2Int{ 0,1 })); // down
			Monster* monster4 = dynamic_cast<Monster*>(scene->GetCreatureAt(GetCellPos() + Vec2Int{ -1,0 })); // left

			if (monster)
			{
				if (monster->GetType() == Protocol::OBJECT_TYPE_PLAYER)
				{
					SetState(IDLE);
					return;
				}

				// 몬스터에 피격 이펙트 출력
				scene->SpawnObject<HitEffect>(GetCellPos() + Vec2Int{ 0,-1 });
				monster->OnDamaged(this);

				if (monster->info.hp() <= 0)
					return;

				monster->SetWait(50);
				monster->SetState(HIT);
				monster->KnockBack(this);
			}

			if (monster2)
			{
				if (monster2->GetType() == Protocol::OBJECT_TYPE_PLAYER)
				{
					SetState(IDLE);
					return;
				}

				// 몬스터에 피격 이펙트 출력
				scene->SpawnObject<HitEffect>(GetCellPos() + Vec2Int{ 1,0 });
				monster2->OnDamaged(this);

				if (monster2->info.hp() <= 0)
					return;

				monster2->SetWait(50);
				monster2->SetState(HIT);
				monster2->KnockBack(this);
			}

			if (monster3)
			{
				if (monster3->GetType() == Protocol::OBJECT_TYPE_PLAYER)
				{
					SetState(IDLE);
					return;
				}

				// 몬스터에 피격 이펙트 출력
				scene->SpawnObject<HitEffect>(GetCellPos() + Vec2Int{ 0,1 });
				monster3->OnDamaged(this);

				if (monster3->info.hp() <= 0)
					return;

				monster3->SetWait(50);
				monster3->SetState(HIT);
				monster3->KnockBack(this);
			}

			if (monster4)
			{
				if (monster4->GetType() == Protocol::OBJECT_TYPE_PLAYER)
				{
					SetState(IDLE);
					return;
				}

				// 몬스터에 피격 이펙트 출력
				scene->SpawnObject<HitEffect>(GetCellPos() + Vec2Int{ -1,0 });
				monster4->OnDamaged(this);

				if (monster4->info.hp() <= 0)
					return;

				monster4->SetWait(50);
				monster4->SetState(HIT);
				monster4->KnockBack(this);
			}
		}
		SetState(MOVE);
		if (GetCellPos() == Vec2Int{ 44, 18 })
			scene->SpawnObject<TeleportEffect>(GetCellPos());
	}
}

void Player::TickTeleport()
{

}

void Player::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;

	case MOVE:
		GET_SINGLE(SoundManager)->Play(L"Land");
		SetFlipbook(_flipbookMove[info.dir()]);
		break;

	case SKILL:
		if (GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
		{
			GET_SINGLE(SoundManager)->Play(L"Sword");
			SetFlipbook(_flipbookAttack[info.dir()]);
		}
		else if (GetWeaponType() == Protocol::WEAPON_TYPE_BOW)
		{
			GET_SINGLE(SoundManager)->Play(L"Bow");
			SetFlipbook(_flipbookBow[info.dir()]);
		}
		else
		{
			GET_SINGLE(SoundManager)->Play(L"Teleport");
			SetFlipbook(_flipbookStaff[info.dir()]);
		}
		break;

	case SPIN_READY:
		if (GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
		{
			GET_SINGLE(SoundManager)->Play(L"SpinReady");
			SetFlipbook(_flipbookSpinReady[info.dir()]);
		}
		break;

	case SPIN:
		if (GetWeaponType() == Protocol::WEAPON_TYPE_SWORD)
		{
			GET_SINGLE(SoundManager)->Play(L"SpinAttack");
			SetFlipbook(_flipbookSpin[info.dir()]);
		}
		break;

	case TELEPORT:
		break;

	default:
		return;
	}
}

void Player::Handle_S_Fire(const Protocol::ObjectInfo& info, uint64 id)
{
	// 화살 BroadCast로 인해 2발씩 생성되는 버그 수정, 서버에서는 50ms 이후 화살 생성
	_now = GetTickCount64();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (_now - _prev >= 50)
	{
		if (this->info.arrows() <= 0)
			return;

		Arrow* arrow = scene->SpawnObject<Arrow>(Vec2Int{ info.posx(),info.posy() });
		arrow->info = info;
		arrow->SetOwner(this);
		this->info.set_arrows(this->info.arrows() - 1);
	}
	_prev = _now;
}

void Player::SyncToServer()
{
	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}

int Player::GetQuestProgress(int questId)
{
	return _questsProgress[questId];
}

void Player::SetQuestProgress(int questId, int progress)
{
	_questsProgress[questId] = progress;
}
