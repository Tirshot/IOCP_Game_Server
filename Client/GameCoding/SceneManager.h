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
	shared_ptr<Scene> GetCurrentScene(){ return _scene; }
	shared_ptr<class DevScene> GetDevScene();

	// 플레이어 조작
	shared_ptr<MyPlayer> GetMyPlayer() { return _myPlayer; }
	uint64 GetMyPlayerId();
	void SetMyPlayer(shared_ptr<MyPlayer> myplayer) { _myPlayer = myplayer; }
	shared_ptr<Player> GetPlayerByID(uint64 objectId);

private:
	shared_ptr<Scene> _scene = nullptr;
	SceneType _sceneType = SceneType::None;
	shared_ptr<MyPlayer> _myPlayer = nullptr;

public:
	// 카메라 조작
	Vec2 GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Vec2 cameraPos) { _cameraPos = cameraPos; }

private:
	Vec2 _cameraPos = { 0,0 };

public:
	void SetPause(bool pause);

};

