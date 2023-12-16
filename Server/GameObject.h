#pragma once
class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();
	
	// 전투 관련 코드


	// 오브젝트를 총괄 관리
	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster();


public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

private:
	// id 부여
	static atomic<uint64> s_idGenerator;
};

