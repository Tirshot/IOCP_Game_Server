#pragma once
class Scene;
class Player;
class MyPlayer;
class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	void Clear();

public:
	// Scene 조작
	void ChangeScene(SceneType sceneType);
	Scene* GetCurrentScene(){ return _scene; }
	class DevScene* GetDevScene();

	// 플레이어 조작
	MyPlayer* GetMyPlayer() { return _myPlayer; }
	uint64 GetMyPlayerId();
	void SetMyPlayer(MyPlayer* myplayer) { _myPlayer = myplayer; }
	Player* GetPlayerByID(uint64 objectId);

private:
	Scene* _scene = nullptr;
	SceneType _sceneType = SceneType::None;
	MyPlayer* _myPlayer = nullptr;

public:
	// 카메라 조작
	Vec2 GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Vec2 cameraPos) { _cameraPos = cameraPos; }

public:
	Vec2 _cameraPos = { 0,0 };

public:
	void SetPause(bool pause);

};

