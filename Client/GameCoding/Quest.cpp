#include "pch.h"
#include "Quest.h"

Quest::Quest()
{
}

Quest::Quest(int questid)
{
	switch (questid)
	{
	case 0:
		info.set_questid(questid);
		_questName = L"�׽�Ʈ��";
		_questDescription = L"�׽�Ʈ�� ��ũ��Ʈ�Դϴ�.";
		info.set_queststate(Q_IDLE);
		info.set_targetnums(10);
		info.set_targettype(Protocol::OBJECT_TYPE_MONSTER);
		break;

	case 1:
		info.set_questid(questid);
		_questName = L"���� ���� ���� ���� ��";
		_questDescription = L"���� �ʹ� ���ƿ�.\n���� 10������ óġ���ֽðھ��?\n��ʴ� �˳��� �帮�ھ��.";
		info.set_queststate(Q_IDLE);
		info.set_targetnums(1);
		info.set_targettype(Protocol::OBJECT_TYPE_MONSTER);
		break;

	default:
		return;
	}
}

Quest::~Quest()
{
}

void Quest::Init()
{
}

void Quest::Accept()
{

}

void Quest::Completed()
{
}
