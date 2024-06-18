#pragma once
#include "ResourceBase.h"

class Texture;

struct FlipbookInfo
{
	shared_ptr<Texture> texture = nullptr;
	wstring name;
	// �� ��������Ʈ �� ĭ�� ũ��
	Vec2Int size = {};
	// �ִϸ��̼� ���� ��ġ, ������ ��ġ, �� ��° �ٿ� �ִ°�
	int32 start = 0;
	int32 end = 0;
	int32 line = 0;
	// �ִϸ��̼� �ӵ�
	float duration = 1.f;
	// �ִϸ��̼� �ݺ�
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

