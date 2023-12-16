#pragma once
class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom();
	virtual ~GameRoom();

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	GameObjectRef FindObject(uint64 id);
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	void AddObject(GameObjectRef gameObject);
	void RemoveObject(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

private:
	// ID를 발급받아 활용
	map<uint64, PlayerRef> _players;
	map<uint64, MonsterRef> _monsters;
};

// 규모가 커지면 매니저 클래스를 파서 사용
extern GameRoomRef GRoom;