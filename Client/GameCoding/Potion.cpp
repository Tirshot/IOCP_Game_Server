#include "pch.h"
#include "Potion.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

Potion::Potion()
{
	_pos = { 83, 82 };
	_potion = GET_SINGLE(ResourceManager)->GetSprite(L"Potion");
	_rect.left = _pos.x + _potion->GetSize().x + 2;
	_rect.right = _rect.left + 30;
	_rect.top = _pos.y + _potion->GetSize().y / 4;
	_rect.bottom = _rect.top + 18;
}

Potion::~Potion()
{
	_myPlayer = nullptr;
	_potion = nullptr;
}

void Potion::BeginPlay()
{
}

void Potion::Tick()
{
	_myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

	if (_myPlayer)
		_potionCount = _myPlayer->GetPotionNums();
}

void Potion::Render(HDC hdc)
{
	// ���� ��������Ʈ
	::TransparentBlt(hdc,
		_pos.x,
		_pos.y,
		_potion->GetSize().x,
		_potion->GetSize().y,
		_potion->GetDC(),
		0,
		0,
		_potion->GetSize().x,
		_potion->GetSize().y,
		_potion->GetTransparent());

	// ���� ���� �ؽ�Ʈ
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	wstring str = std::format(L"{0}", _potionCount);
	::DrawTextW(hdc, str.c_str(), -1, &_rect, DT_LEFT);
}
