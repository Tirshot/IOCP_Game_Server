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

	void OnClickShopButton(); // ���� UI Ȱ��
	void OnClickQuestButton(); // ����Ʈ UI Ȱ��
	void OnClickExitButton();

protected:
	RECT _rect = {};
	int _alpha = 0;
	float _sumTime = 0.f;
	class Sprite* _merchantSprite = nullptr;
};
// AddChild�� ���� ��ư, ui, textbox�� _children�� �߰��Ͽ� ����Ѵ�.
