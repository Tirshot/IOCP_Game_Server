#pragma once
#include "Panel.h"
class Texture;
class MerchantUI :  public Panel
{
	using Super = Panel;

public:
	MerchantUI();
	virtual ~MerchantUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void OnClickShopButton(); // 상점 UI 활성
	void OnClickQuestButton(); // 퀘스트 UI 활성
	void OnClickExitButton();

protected:
	RECT _rect = {};
	int _alpha = 0;
	float _sumTime = 0.f;
	class Sprite* _merchantSprite = nullptr;
};
// AddChild를 통해 버튼, ui, textbox를 _children에 추가하여 사용한다.
