#pragma once
#include "ResourceBase.h"

class Texture;

class Sprite : public ResourceBase
{
public:
	// �ؽ����� ������ ����
	Sprite();
	Sprite(shared_ptr<Texture> texture, int32 x, int32 y, int32 cx, int32 cy);
	virtual ~Sprite();

	HDC		GetDC();
	int32	GetTransparent();
	Vec2Int GetPos() { return Vec2Int{ _x, _y }; }
	Vec2Int GetSize() { return Vec2Int{ _cx, _cy }; }

private:
	shared_ptr<Texture> _texture = nullptr;
	int32 _x = 0;
	int32 _y = 0;
	int32 _cx = 0;
	int32 _cy = 0;
};

