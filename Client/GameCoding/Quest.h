#pragma once
class Quest
{
public:
	Quest();
	Quest(int questid);
	virtual ~Quest();

	virtual void Init();
	virtual void Accept();
	virtual void Completed();

public:
	Protocol::QuestInfo info;
	wstring _questName = L"";
	wstring _questDescription = L"";
};

