#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "TitleScene.h"
#include "Player.h"
#include "MyPlayer.h"

void SceneManager::Init()
{

}

void SceneManager::Update()
{
	if (_scene)
		_scene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (_scene)
		_scene->Render(hdc);
}

void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	// �����Ϸ��� Scene�� ���� Scene�� ��ġ�ϸ� ����
	//if (_sceneType == sceneType)
	//	return;

	// �����Ϸ��� Scene�� newScene�̶� ���
	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DevScene:
		newScene = new DevScene;
		break;

	case SceneType::TitleScene:
		newScene = new TitleScene;
		break;

	}

	// Scene ������ ���� ���� scene ����
	if (_scene)
		SAFE_DELETE(_scene);

	newScene->SetPauseState(false);
	// ���� Scene�� �����Ϸ��� Scene���� ��ü
	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}

class DevScene* SceneManager::GetDevScene()
{
	return dynamic_cast<DevScene*>(GetCurrentScene());
}

uint64 SceneManager::GetMyPlayerId()
{
	return _myPlayer ? _myPlayer->info.objectid() : 0;
}

Player* SceneManager::GetPlayerByID(uint64 objectId)
{
	if (!_scene)
		return nullptr;

	Player* player = _scene->GetPlayerByID(objectId);

	return player ? player : nullptr;
}

void SceneManager::SetPause(bool pause)
{
	GetDevScene()->SetPauseState(pause);
}
