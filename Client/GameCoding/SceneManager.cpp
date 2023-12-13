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
	// �����Ϸ��� Scene�� ���� Scene�� ��ġ�ϸ� ����
	if (_sceneType == sceneType)
		return;

	// �����Ϸ��� Scene�� newScene�̶� ���
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

	// Scene ������ ���� ���� scene ����
	SAFE_DELETE(_scene);

	// ���� Scene�� �����Ϸ��� Scene���� ��ü
	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}
