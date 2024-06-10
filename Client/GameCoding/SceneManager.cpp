#include "pch.h"
#include "SceneManager.h"
#include "ResourceManager.h"
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
	/*SAFE_DELETE(_scene);*/
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	if (_scene)
		_scene->SetPauseState(true);

	// 변경하려는 Scene과 현재 Scene이 일치하면 종료
	//if (_sceneType == sceneType)
	//	return;

	// 변경하려는 Scene을 newScene이라 명명
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

	newScene->SetPauseState(false);

	// 기존 Scene을 변경하려는 Scene으로 대체
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
	GetDevScene()->SetPauseState(pause);
}
