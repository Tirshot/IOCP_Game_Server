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
	void SetRewardItem(int itemID, int num = 1);
	void SetRewardGold(int gold);
	void ResetPage() { _page = 0; }

private:
	void VisibleButton();
	void OnClickAcceptButton();
	void OnClickDeclineButton();
	void OnClickConfirmButton();

protected:
	RECT _rect = {};
	vector<wstring> _scripts = {};
	int _questID = 0;
	Protocol::QUEST_STATE _questState = Protocol::QUEST_STATE_IDLE;
	int _alpha = 0;
	shared_ptr<class Sprite> _merchantSprite = nullptr;
	shared_ptr<class Sprite> _backGround = nullptr;
	int _page = 0;		// ��ȭ â ������
	int _maxPage = 0;	// ��ȭ â ������ �� ����

private:
	int _rewardItem = 0;
	int _rewardItemNum = 0;
	int _rewardGold = 0;
	bool _rewardVisible = false;
};