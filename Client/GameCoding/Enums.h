#pragma once
enum class SceneType
{
		None,
		DevScene,
		EditScene,
		TitleScene,
		LoadScene,
};

enum class SoundType
{
	NONE = 0,
	BGM,
	UI,
	Effect,
};

enum class CreatureType
{
	None,
	Player,
	Monster,
};

enum class TextAlign
{
	Left,
	Center,
	Right,
};

// 정수 변환을 건너뛰기 위해 enum을 사용
enum LAYER_TYPE
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_EFFECT,
	LAYER_UI,
	LAYER_MAXCOUNT,
};

enum class ColliderType : uint8
{
	Box,
	Sphere,
};

enum COLLISION_LAYER_TYPE
{
	CLT_OBJECT,
	CLT_GROUND,
	CLT_WALL,
};

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
#define CHAT Protocol::OBJECT_STATE_TYPE_CHAT
#define SPIN_READY Protocol::OBJECT_STATE_TYPE_SPIN_READY
#define SPIN Protocol::OBJECT_STATE_TYPE_SPIN
#define TELEPORT Protocol::OBJECT_STATE_TYPE_TELEPORT

#define Q_IDLE Protocol::QUEST_STATE_IDLE
#define Q_ACCEPT Protocol::QUEST_STATE_ACCEPT
#define Q_COMPLETED Protocol::QUEST_STATE_COMPLETED

#define ARROW Protocol::ITEM_TYPE_ARROW
#define POTION Protocol::ITEM_TYPE_POTION
#define MAXHEART Protocol::ITEM_TYPE_MAXHEART
#define HEART Protocol::ITEM_TYPE_HEART
#define FULLHEART Protocol::ITEM_TYPE_FULLHEART

#define CONSUMABLE Protocoll::ITEM_TYPE_CONSUMABLE
#define WEARABLE Protocoll::ITEM_TYPE_WEARABLE

#define WEAPON Protocoll::WEARABLE_TYPE_WEAPON
#define HELMET Protocoll::WEARABLE_TYPE_HELMET
#define ARMOR Protocoll::WEARABLE_TYPE_ARMOR
#define PANTS Protocoll::WEARABLE_TYPE_PANTS
#define BOOTS Protocoll::WEARABLE_TYPE_BOOTS

using ObjectState = Protocol::OBJECT_STATE_TYPE;

enum class WeaponType
{
	Sword,
	Bow,
	Staff
};

struct TABLE
{
	int ItemId;
	string Name;
	string Description;
};

struct ITEM
{
	int ItemId = 0;
	int ItemCount = 0;
	int MaxCount = 0;
	wstring Name = L"";
	wstring KorName = L"";
	wstring Type = L"";
	wstring SubType = L"";
	wstring Description = L"";
	int Price = 0;
	shared_ptr<class Sprite> Sprite = nullptr;
	RECT Rect = {};
	int index = 0;
	
	bool operator== (shared_ptr<ITEM> other)
	{
		if (this->Rect.left == other->Rect.left
			&& this->Rect.right == other->Rect.right
			&& this->Rect.top == other->Rect.top
			&& this->Rect.bottom == other->Rect.bottom)
			return true;

		return false;
	}

	void Reset()
	{
		this->ItemId = 0;
		this->ItemCount = 0;
		this->KorName = L"";
		this->Name = L"";
		this->Description = L"";
		this->Price = 0;
		this->Sprite = nullptr;
		this->SubType = L"";
		this->Type = L"";
	}
};