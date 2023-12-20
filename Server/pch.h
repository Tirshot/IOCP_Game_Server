#pragma once

#include "CorePch.h"

using GameSessionRef = shared_ptr<class GameSession>;
using GameObjectRef = shared_ptr<class GameObject>;
using CreatureRef = shared_ptr<class Creature>;
using GameRoomRef = shared_ptr<class GameRoom>;
using PlayerRef = shared_ptr<class Player>;
using MonsterRef = shared_ptr<class Monster>;

#include "Enum.pb.h"
using Dir = Protocol::DIR_TYPE;

#define DIR_DOWN Protocol::DIR_TYPE_DOWN
#define DIR_UP Protocol::DIR_TYPE_UP
#define DIR_LEFT Protocol::DIR_TYPE_LEFT
#define DIR_RIGHT Protocol::DIR_TYPE_RIGHT

#define IDLE Protocol::OBJECT_STATE_TYPE_IDLE
#define MOVE Protocol::OBJECT_STATE_TYPE_MOVE
#define SKILL Protocol::OBJECT_STATE_TYPE_SKILL

using ObjectState = Protocol::OBJECT_STATE_TYPE;

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "ServerPacketHandler.h"

struct VectorInt
{
	VectorInt() {}
	VectorInt(int32 x, int32 y) : x(x), y(y) { }
	VectorInt(POINT pt) : x((int32)pt.x), y((int32)pt.y) { }

	VectorInt operator+(const VectorInt& other)
	{
		VectorInt ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}
	VectorInt operator-(const VectorInt& other)
	{
		VectorInt ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	bool operator<(const VectorInt& other) const
	{
		if (x != other.x)
			return x < other.x;

		return y < other.y;
	}

	bool operator>(const VectorInt& other) const
	{
		if (x != other.x)
			return x > other.x;

		return y > other.y;
	}

	void operator+=(const VectorInt& other)
	{
		x += other.x;
		y += other.y;
	}
	bool operator==(const VectorInt& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const VectorInt& other)
	{
		return x != other.x || y != other.y;
	}
	void operator-=(const VectorInt& other)
	{
		x -= other.x;
		y -= other.y;
	}
	float LengthSquared()
	{
		// 루트 내부를 계산
		return x * x + y * y;
	}
	float Length()
	{
		// 제곱근을 계산
		return sqrt(LengthSquared());
	}

	int32 x = (int32)0.f;
	int32 y = (int32)0.f;

};
using Vec2Int = VectorInt;