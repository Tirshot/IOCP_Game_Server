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
        // 로딩이 완료되면 다음 씬으로 전환
        if (_nextSceneType != SceneType::None)
            GET_SINGLE(SceneManager)->ChangeScene(_nextSceneType);
    }
}

void LoadScene::Render(HDC hdc)
{

}
