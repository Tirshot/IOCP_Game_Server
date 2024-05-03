#pragma once
#include "Panel.h"
class Texture;
class MerchantDialogueUI : public Panel
{
	using Super = Panel;

public:
	MerchantDialogueUI();
	virtual ~MerchantDialogueUI();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

public:
	void SetDialogue(int questID);
	void SetRewardItem(int itemID);
	void SetRewardGold(int gold);
	void ResetPage() { _page = 0; }

private:
	void OnClickAcceptButton();
	void OnClickDeclineButton();

protected:
	RECT _rect = {};
	vector<wstring> _scripts = {};
	int _questID = 0;
	Protocol::QUEST_STATE _questState;
	int _alpha = 0;
	class Sprite* _merchantSprite = nullptr;
	int _page = 0;		// ��ȭ â ������
	int _maxPage = 0;	// ��ȭ â ������ �� ����

private:
	class ITEM* _item = nullptr;
	int _gold = 0;
};