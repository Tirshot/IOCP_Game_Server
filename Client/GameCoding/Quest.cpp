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
		_questName = L"테스트용";
		_questDescription = L"테스트용 스크립트입니다.";
		info.set_queststate(Q_IDLE);
		info.set_targetnums(10);
		info.set_targettype(Protocol::OBJECT_TYPE_MONSTER);
		break;

	case 1:
		info.set_questid(questid);
		_questName = L"몸에 좋고 맛도 좋은 뱀";
		_questDescription = L"뱀이 너무 많아요.\n뱀을 10마리만 처치해주시겠어요?\n사례는 넉넉히 드리겠어요.";
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
