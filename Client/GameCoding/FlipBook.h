#pragma once
#include "ResourceBase.h"

class Texture;

struct FlipbookInfo
{
	shared_ptr<Texture> texture = nullptr;
	wstring name;
	// 각 스프라이트 한 칸의 크기
	Vec2Int size = {};
	// 애니메이션 시작 위치, 마지막 위치, 몇 번째 줄에 있는가
	int32 start = 0;
	int32 end = 0;
	int32 line = 0;
	// 애니메이션 속도
	float duration = 1.f;
	// 애니메이션 반복
	bool loop = true;
};


class Flipbook : public ResourceBase
{
public:
	Flipbook();
	virtual ~Flipbook();

	void SetInfo(const FlipbookInfo& info) { _info = info; }
	const FlipbookInfo& GetInfo() { return _info; }

private:
	FlipbookInfo _info;
};

