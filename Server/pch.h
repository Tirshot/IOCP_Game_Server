#pragma once

#include "CorePch.h"
using GameSessionRef = shared_ptr<class GameSession>;
using GameObjectRef = shared_ptr<class GameObject>;
using CreatureRef = shared_ptr<class Creature>;
using GameRoomRef = shared_ptr<class GameRoom>;
using PlayerRef = shared_ptr<class Player>;
using MonsterRef = shared_ptr<class Monster>;
using SnakeRef = shared_ptr<class Snake>;
using MoblinRef = shared_ptr<class Moblin>;
using NPCRef = shared_ptr<class NPC>;
using SignRef = shared_ptr<class Sign>;
using ItemRef = shared_ptr<class Item>;
using NameChangerRef = shared_ptr<class NameChanger>;
using ProjectileRef = shared_ptr<class Projectile>;
using ArrowRef = shared_ptr<class Arrow>;
using ChatRef = shared_ptr<class Chat>;
using QuestRef = shared_ptr<class Quest>;
using ShopItemRef = shared_ptr<class ShopItem>;
using InventoryRef = shared_ptr<class Inventory>;
using TriggerRef = shared_ptr<class Trigger>;

#include "Enum.pb.h"
using Dir = Protocol::DIR_TYPE;

#define DIR_DOWN Protocol::DIR_TYPE_DOWN
#define DIR_UP Protocol::DIR_TYPE_UP
#define DIR_LEFT Protocol::DIR_TYPE_LEFT
#define DIR_RIGHT Protocol::DIR_TYPE_RIGHT

#define IDLE Protocol::OBJECT_STATE_TYPE_IDLE
#define MOVE Protocol::OBJECT_STATE_TYPE_MOVE
#define SKILL Protocol::OBJECT_STATE_TYPE_SKILL
#define HIT Protocol::OBJECT_STATE_TYPE_HIT
#define STOP Protocol::OBJECT_STATE_TYPE_STOP
#define SPIN Protocol::OBJECT_STATE_TYPE_SPIN
#define TELEPORT Protocol::OBJECT_STATE_TYPE_TELEPORT

#define Q_IDLE Protocol::QUEST_STATE_IDLE
#define Q_ACCEPT Protocol::QUEST_STATE_ACCEPT
#define Q_COMPLETED Protocol::QUEST_STATE_COMPLETED

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

	VectorInt operator-()
	{
		VectorInt ret;
		ret.x = -x;
		ret.y = -y;
		return ret;
	}
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
	VectorInt operator*(int other)
	{
		VectorInt ret;
		ret.x = x * other;
		ret.y = y * other;
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
	VectorInt Dot(VectorInt& vec, int other)
	{
		VectorInt ret;
		ret.x = vec.x * other;
		ret.y = vec.y * other;
		return ret;
	}

	int32 x = (int32)0.f;
	int32 y = (int32)0.f;

};
using Vec2Int = VectorInt;

#define DECLARE_SINGLE(classname)						\
private:																	\
	classname() {  }													\
public:																	\
	static classname* GetInstance()							\
	{																		\
		static classname s_instance;							\
		return &s_instance;											\
	}																		

#define GET_SINGLE(classname)		classname::GetInstance()