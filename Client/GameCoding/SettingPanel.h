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

	// 음소거 체크박스
	// 볼륨 슬라이더
	// 창 닫기 버튼

private:
	Sprite* _background = nullptr;
	Sprite* _soundSettings = nullptr;
	Sprite* _BGM = nullptr;
	Sprite* _UI = nullptr;
	Sprite* _SFX = nullptr;
};

