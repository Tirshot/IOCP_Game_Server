#pragma once
class Scene;
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
	void ChangeScene(SceneType sceneType);
	Scene* GetCurrentScene(){ return _scene; }

	class DevScene* GetDevScene();

	MyPlayer* GetMyPlayer() { return _myPlayer; }
	uint64 GetMyPlayerId();
	void SetMyPlayer(MyPlayer* myplayer) { _myPlayer = myplayer; }

private:
		Scene* _scene = nullptr;
		SceneType _sceneType = SceneType::None;
		MyPlayer* _myPlayer = nullptr;

public:
	Vec2 GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Vec2 cameraPos) { _cameraPos = cameraPos; }

public:
	Vec2 _cameraPos = { 0,0 };


};

