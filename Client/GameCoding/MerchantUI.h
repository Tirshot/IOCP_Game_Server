#pragma once
#include "Panel.h"
class Sprite;
class MerchantUI :  public Panel
{
	using Super = Panel;

public:
	MerchantUI();
	virtual ~MerchantUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void OnClickBackButton();
	void OnClickShopButton(); // ���� UI Ȱ��
	void OnClickQuestButton(); // ����Ʈ UI Ȱ��
	void OnClickExitButton();

protected:
	RECT _rect = {};
	Sprite* _background = nullptr;
	int _alpha = 0;
	float _sumTime = 0.f;
};
// AddChild�� ���� ��ư, ui, textbox�� _children�� �߰��Ͽ� ����Ѵ�.
