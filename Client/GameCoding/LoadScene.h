#pragma once
#include "Scene.h"
class LoadScene : public Scene
{
public:
    LoadScene();
    virtual ~LoadScene() override;

    void Init() override;
    void Update() override;
    void Render(HDC hdc) override;

private:
    SceneType _nextSceneType = SceneType::None;
    float _progress = 0.f;
    bool _initialized = false;
};

