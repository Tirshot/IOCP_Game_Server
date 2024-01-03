#include "pch.h"
#include "Chat.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "TimeManager.h"

Chat::Chat()
{
	
}

Chat::Chat(Vec2 rectPos)
{
	_rectPos.x = rectPos.x;
	_rectPos.y = rectPos.y;
}

Chat::~Chat()
{
	RemoveChild(this);
}

void Chat::BeginPlay()
{
	AddChild(this);
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
	// ������ �� ���̵� ��, ����� �� ���̵� �ƿ�
	ChatBoxFade();
}

void Chat::Render(HDC hdc)
{
	
	BLENDFUNCTION bf = {};
	bf.AlphaFormat = 0; //�Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER; //  ������ ��� �̹����� ��ħ
	bf.SourceConstantAlpha = _alpha; // ����(���� 0 - ������ 255)

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
		int maxLineWidth = _rect.right - _rect.left - 20;

		// 1 line per 19 korean spells
		for (int i = 0; i < _texts.size(); i++)
		{
			// ���ڿ��� Ư�� ���̸� �Ѿ�� �ٹٲ� �߰�
			for (size_t pos = maxLineWidth; pos < _texts[i].length(); pos += maxLineWidth)
			{
				_texts[i].insert(pos, L"\n");
			}

			RECT textRect = {};
			textRect.left = _rect.left + 10;
			textRect.right = _rect.right - 10;
			textRect.top = _rect.top + 10 + i * 36;
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

void Chat::ChatBoxFade()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += deltaTime;

	// �ؽ�Ʈ�� ���� ������ â�� ������ ����
	if (_texts.empty() == false)
	{
		_visible = true;
		_alpha++;
		_alpha = clamp(_alpha, 50, 200);
	}

	// 1.5�� �� �ؽ�Ʈ�� �����ϰ� â�� ����
	if (_sumTime >= 1.5f)
	{
		if (_texts.empty() == false)
		{
			_texts.erase(_texts.begin());
			_sumTime = 0;
		}
		// â�� ���̵� �ƿ�
		_alpha--;
		_alpha = clamp(_alpha, 0, 200);
	}

	// â�� ����� ��ο�����
	if (_alpha < 10)
	{
		_visible = false;
		_sumTime = 0;
	}
}

void Chat::SetVisibleChat()
{
	SetVisible(true);
	SetAlpha(200);
	SetSumTime(0);
}
