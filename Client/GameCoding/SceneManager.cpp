#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"

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
	// 변경하려는 Scene과 현재 Scene이 일치하면 종료
	if (_sceneType == sceneType)
		return;

	// 변경하려는 Scene을 newScene이라 명명
	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DevScene:
		newScene = new DevScene;
		break;

	//case SceneType::EditScene:
	//	newScene = new EditScene;
	//	break;

	}

	// Scene 변경을 위해 기존 scene 제거
	SAFE_DELETE(_scene);

	// 기존 Scene을 변경하려는 Scene으로 대체
	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}
