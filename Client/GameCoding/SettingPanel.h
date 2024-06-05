#pragma once
#include "Panel.h"

class Sprite;
class SettingPanel : public Panel
{
	using Super = Panel;

public:
	SettingPanel();
	virtual ~SettingPanel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

private:
	void OnClickCrossButton();
	void SetAllVolume();
	void SetBGMVolume();
	void SetSFXVolume();
	void SetUIVolume();

	// ���Ұ� üũ�ڽ�
	// ���� �����̴�
	// â �ݱ� ��ư

private:
	Sprite* _background = nullptr;
	Sprite* _soundSettings = nullptr;
	Sprite* _BGM = nullptr;
	Sprite* _UI = nullptr;
	Sprite* _SFX = nullptr;
};

