#include "pch.h"
#include "Chat.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "TimeManager.h"


Chat::Chat()
{
	AddChild(this);
}

Chat::Chat(Vec2 rectPos)
{
	AddChild(this);
	_rectPos.x = rectPos.x;
	_rectPos.y = rectPos.y;
}

Chat::~Chat()
{
	RemoveChild(this);
}

void Chat::BeginPlay()
{
	// size of chat
	_rect = GetRect();
	_rect.left = 0;
	_rect.right = 300;
	_rect.top = 0;
	_rect.bottom = 200;

	SetRectPos((int)_rectPos.x, (int)_rectPos.y);

	_chatSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");
}

void Chat::Tick()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += deltaTime;

	// �ؽ�Ʈ�� ���� ������ â�� ������ ����
	if (_texts.empty() == false)
		_visible = true;

	// 3�� �� �ؽ�Ʈ�� �����ϰ� â�� ����
	if (_sumTime >= 3.f)
	{
		if (_texts.empty() == false)
		{
			auto removeIt = std::remove(_texts.begin(), _texts.end(), _texts[0]);
			_texts.erase(removeIt, _texts.end());
		}
		_visible = false;
		_sumTime = 0;
	}
}

void Chat::Render(HDC hdc)
{
	BLENDFUNCTION bf = {};
	bf.AlphaFormat = 0; //�Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER; //  ������ ��� �̹����� ��ħ
	bf.SourceConstantAlpha = 127; // ����(���� 0 - ������ 255)

	if (_visible)
	{
		// ��׶���
		{
			::AlphaBlend(hdc,
				(int)_pos.x,
				(int)_pos.y,
				_rect.right - _rect.left,
				_rect.bottom - _rect.top,
				_chatSprite->GetDC(),
				0,
				0,
				100,
				100,
				bf);
		}

		// �ؽ�Ʈ
		if (_texts.empty() == true)
			return;

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(230, 230, 230));

		for (int i = 0; i < _texts.size(); i++)
		{/*
			::TextOut(hdc,
				(int)GetPos().x,
				(int)GetPos().y + i * 18,
				_texts[i].c_str(),
				_texts[i].size());*/

			RECT textRect;
			textRect.left = _rect.left;
			textRect.right = _rect.right;
			textRect.top = _rect.top + i * 18;
			textRect.bottom = _rect.bottom;

			// DrawText �Լ��� ����Ͽ� �ؽ�Ʈ ���
			DrawText(hdc, _texts[i].c_str(), -1, &textRect, DT_LEFT | DT_WORDBREAK);
		}
	}
}

void Chat::SetRectPos(int posX, int posY)
{
	_rect.left = posX;
	_rect.right += posX;
	_rect.top = posY;
	_rect.bottom += posY;
}

void Chat::AddText(const wstring str)
{
	_texts.push_back(str);
	// next line
}

