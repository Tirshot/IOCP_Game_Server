#include "pch.h"
#include "LoadScene.h"
#include "SceneManager.h"

LoadScene::LoadScene()
{
}

LoadScene::~LoadScene()
{
}

void LoadScene::Init()
{

}

void LoadScene::Update()
{
    if (_initialized == true)
    {
        // �ε��� �Ϸ�Ǹ� ���� ������ ��ȯ
        if (_nextSceneType != SceneType::None)
            GET_SINGLE(SceneManager)->ChangeScene(_nextSceneType);
    }
}

void LoadScene::Render(HDC hdc)
{

}
