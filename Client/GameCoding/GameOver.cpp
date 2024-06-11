#include "pch.h"
#include "Game.h"
#include "GameOver.h"
#include "Sprite.h"
#include "Button.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "ResourceManager.h"
#include "ItemManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

GameOver::GameOver()
{
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 800;
	_rect.top = 0;
	_rect.bottom = 600;

	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");
	_gameOver = GET_SINGLE(ResourceManager)->GetSprite(L"GameOver");
}

GameOver::~GameOver()
{

}

void GameOver::BeginPlay()
{
	{ // 부활 버튼
		shared_ptr<Button> revive = make_shared<Button>();
		revive->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Revive_Off"), BS_Default);
		revive->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Revive_On"), BS_Pressed);
		revive->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Revive_Hovered"), BS_Hovered);
		revive->SetPos(Vec2{ 200, 400 });
		revive->SetSize({ 140, 40 });
		revive->SetVisible(false);
		revive->AddOnClickDelegate(this, &GameOver::OnClickReviveButton);
		AddChild(revive);
	}

	{ // 게임 종료
		shared_ptr<Button> exit = make_shared<Button>();
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_Default);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_Pressed);
		exit->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Hovered"), BS_Hovered);
		exit->SetPos(Vec2{ 600, 400 });
		exit->SetSize({ 140, 40 });
		exit->SetVisible(false);
		exit->AddOnClickDelegate(this, &GameOver::OnClickExitButton);
		AddChild(exit);
	}

	for (auto& child : _children)
		child->BeginPlay();
}

void GameOver::Tick()
{
	// Game Over UI 페이드 인
	if (_visible)
	{
		FadeIn();
	}

	if (_alpha >= 220)
	{
		auto scene = GET_SINGLE(SceneManager)->GetDevScene();

		if (_invisibleUIs == false)
		{
			_invisibleUIs = true;

			if (scene)
			{
				auto uis = scene->GetUIs();

				if (uis.empty() == false)
				{
					for (auto& ui : uis)
					{
						ui->SetVisible(false);
					}
				}
			}
		}

		SetVisible(true);
		for (auto& child : _children)
		{
			if (child == nullptr)
				return;

			child->SetVisible(true);
			child->Tick();
		}
	}
}

void GameOver::Render(HDC hdc)
{
	// 게임 오버 UI 노출 (투명도)
	BLENDFUNCTION bf = {};
	bf.AlphaFormat = 0; //일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER; //  원본과 대상 이미지를 합침
	bf.SourceConstantAlpha = _alpha; // 투명도(투명 0 - 불투명 255)

	if (_visible)
	{
		// 백그라운드
		{
			::AlphaBlend(hdc,
				_rect.left,
				_rect.top,
				_rect.right - _rect.left,
				_rect.bottom - _rect.top,
				_background->GetDC(),
				0,
				0,
				100,
				100,
				bf);
		}

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(230, 230, 230));

		for (auto& child : _children)
			child->Render(hdc);

		if (_alpha >= 220)
		{ // Game Over
			::TransparentBlt(hdc,
				200,
				100,
				_gameOver->GetSize().x,
				_gameOver->GetSize().y,
				_gameOver->GetDC(),
				0,
				0,
				_gameOver->GetSize().x,
				_gameOver->GetSize().y,
				_gameOver->GetTransparent());
		}
	}
}

void GameOver::FadeIn()
{
	_alpha++;
	_alpha = clamp(_alpha, 0, 230);
}

void GameOver::OnClickReviveButton()
{
	auto scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene == nullptr)
		return;

	auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (myPlayer == nullptr)
		return;

	// 수행 후
	SetVisible(false);
	_invisibleUIs = false;

	// 부활 패킷 전송
	{
		SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Revive(myPlayer->info);
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}
	GET_SINGLE(ItemManager)->Init();
}

void GameOver::OnClickExitButton()
{
	if (_visible)
	{
		// 수행 후 ui 제거
		auto scene = GET_SINGLE(SceneManager)->GetDevScene();
		if (scene)
		{
			scene->RemoveUI(shared_from_this());
		}

		// 프로그램 종료
		PostQuitMessage(0);
	}
}
