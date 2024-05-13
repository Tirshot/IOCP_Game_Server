#include "pch.h"
#include "UI.h"
#include "InputManager.h"
#include "Panel.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Chat.h"
#include "PopUp.h"

int UI::ui_idGenerator = 1;

UI::UI()
{
	_id = ui_idGenerator++;
}

UI::~UI()
{

}

void UI::BeginPlay()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
		for (UI* ui : scene->_uis)
			ui->BeginPlay();
}

void UI::Tick()
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	for (UI* ui : scene->_uis)
		ui->Tick();
}

void UI::Render(HDC hdc)
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	for (UI* ui : scene->_uis)
		ui->Render(hdc);
}

RECT UI::GetRect()
{
	RECT rect =
	{
		(int)_pos.x - (int)_size.x / 2,
		(int)_pos.y - (int)_size.y / 2,
		(int)_pos.x + (int)_size.x / 2,
		(int)_pos.y + (int)_size.y / 2,
	};

	return rect;
}

bool UI::IsMouseInRect()
{
	RECT rect = GetRect();

	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

	if (mousePos.x < rect.left)
		return false;
	if (mousePos.x > rect.right)
		return false;
	if (mousePos.y < rect.top)
		return false;
	if (mousePos.y > rect.bottom)
		return false;

	return true;

	// return ::PtInRect(&rect, mousePos);
}

bool UI::IsMouseInRect(RECT rect)
{
	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
	
	return ::PtInRect(&rect, mousePos);
}

bool UI::IsMouseOutRect(RECT rect)
{
	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

	if (mousePos.x < rect.left)
		return true;
	if (mousePos.x > rect.right)
		return true;
	if (mousePos.y < rect.top)
		return true;
	if (mousePos.y > rect.bottom)
		return true;

	return false;
}
