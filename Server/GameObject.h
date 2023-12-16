#pragma once
class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();
	
	// ���� ���� �ڵ�


	// ������Ʈ�� �Ѱ� ����
	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster();


public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

private:
	// id �ο�
	static atomic<uint64> s_idGenerator;
};

