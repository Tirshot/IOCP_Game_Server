#pragma once
#include <cmath>
#include <windows.h>
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using ServerSessionRef = shared_ptr<class ServerSession>;


struct Vector
{
	Vector() {}
	Vector(float x, float y) : x(x), y(y) { }
	Vector(POINT pt) : x((float)pt.x), y((float)pt.y) { }
	 
	Vector operator+(const Vector& other)
	{
		Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}
	Vector operator-(const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator*(float value)
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(float value)
	{
		x *= value;
		y *= value;
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

	void Normalize()
	{
		float length = Length();

		if (length < 0.0000000000f)
			return;

		x /= length;
		y /= length;
	}

	float Dot(Vector other)
	{
		return x * other.x + y * other.y;
	}

	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	float x = 0.f;
	float y = 0.f;
		
};

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
		return x == other.x  && y == other.y;
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

	float Dot(Vector other)
	{
		return x * other.x + y * other.y;
	}

	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	int32 x = (int32)0.f;
	int32 y = (int32)0.f;

};
using Pos = Vector;
using Vec2 = Vector;
using Vec2Int = VectorInt;

struct Stat
{
	int32 hp = 0;
	int32 maxHp = 0;
	int32 attack = 0;
	int32 defence = 0;
	float speed = 0.f;
};