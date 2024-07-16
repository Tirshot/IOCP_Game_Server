#include "pch.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "NetworkManager.h"
#include "DevScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
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
	_scene = nullptr;
	_myPlayer = nullptr;
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	// �����Ϸ��� Scene�� ���� Scene�� ��ġ�ϸ� ����
	//if (_sceneType == sceneType)
	//	return;
	GET_SINGLE(ResourceManager)->Clear();

	// �����Ϸ��� Scene�� newScene�̶� ���
	shared_ptr<Scene> newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DevScene:
		newScene = make_shared<DevScene>();
		break;

	case SceneType::LoadScene:
		newScene = make_shared <LoadScene>();
		break;

	case SceneType::TitleScene:
		newScene = make_shared <TitleScene>();
		break;
	}

	if (newScene == nullptr)
		return;

	newScene->SetInitialized(false);

	// ���� Scene�� �����Ϸ��� Scene���� ��ü
	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}

shared_ptr<DevScene> SceneManager::GetDevScene()
{
	return dynamic_pointer_cast<DevScene>(GetCurrentScene());
}

uint64 SceneManager::GetMyPlayerId()
{
	return _myPlayer ? _myPlayer->info.objectid() : 0;
}

shared_ptr<Player> SceneManager::GetPlayerByID(uint64 objectId)
{
	if (!_scene)
		return nullptr;

	auto player = _scene->GetPlayerByID(objectId);

	return player ? player : nullptr;
}

void SceneManager::SetPause(bool pause)
{
	GetCurrentScene()->SetPauseState(pause);
}
