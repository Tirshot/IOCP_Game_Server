#include "pch.h"
#include "MiniMap.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyPlayer.h"
#include "DevScene.h"

MiniMap::MiniMap()
{
	_map = GET_SINGLE(ResourceManager)->GetSprite(L"Stage01_mini");
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Chat");
}

MiniMap::~MiniMap()
{
}

void MiniMap::BeginPlay()
{
    Super::BeginPlay();
}

void MiniMap::Tick()
{
    Super::Tick();
}

void MiniMap::Render(HDC hdc)
{
    Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

    if (myPlayer == nullptr)
        return;

    auto pos = myPlayer->GetPos();
    auto cellPos = myPlayer->GetCellPos();

    // 배경 그리기
    ::StretchBlt(hdc,
        _pos.x - 3,
        _pos.y,
        _size.x + 3,
        _size.y + 21,
        _background->GetDC(),
        0,
        0,
        100,
        100,
        SRCCOPY);

    // 맵 그리기
    float width = _map->GetSize().x;
    float height = _map->GetSize().y;
    float scaleWidth = width / (_scale * _scale);
    float scaleHeight = height / (_scale * _scale);

    float srcX = (pos.x - scaleWidth)  / _scale;
    float srcY = (pos.y - scaleHeight) / _scale;

    ::StretchBlt(hdc,
        _pos.x,
        _pos.y,
        _size.x,
        _size.y,
        _map->GetDC(),
        srcX,
        srcY,
        scaleWidth,
        scaleHeight,
        SRCCOPY);

    auto scene = GET_SINGLE(SceneManager)->GetCurrentScene();
    if (scene)
    {
        auto actors = scene->GetActors();
        if (actors.empty() == false)
        {
            for (const auto& actor : actors)
            {
                auto actorPos = actor->GetPos();
                auto Vec = (actorPos - pos);

                // 정확한 위치 계산
                float screenX = _pos.x + (_size.x / 2.f) + (Vec.x / _scale) / (_scale * 2);
                float screenY = _pos.y + (_size.y / 2.f) + (Vec.y / _scale) / (_scale);

                // 컬링
                if (screenX < _pos.x || screenX > _pos.x + _size.x || screenY < _pos.y || screenY > _pos.y + _size.y)
                    continue;

                ::BitBlt(hdc,
                   screenX,
                   screenY,
                    5,
                    5,
                    _background->GetDC(),
                    5,
                    5,
                    WHITENESS);
            }
        }
    }

    // 좌표
    wstring text = format(L"({0}, {1})", cellPos.x, cellPos.y);
    RECT textRect = { _pos.x, _pos.y + _size.y, _pos.x + _size.x, _pos.y + _size.y + 20};

    SetTextColor(hdc, RGB(255, 255, 255));
    DrawTextW(hdc, text.c_str(), -1, &textRect, DT_RIGHT | DT_VCENTER | DT_WORDBREAK);

    // 안전구역 표시
    wstring safeZoneText = L"";

    bool bSafeZone = myPlayer->IsSafeZone(myPlayer->GetCellPos());

    if (bSafeZone)
    {
        safeZoneText = L"안전 구역";
        SetTextColor(hdc, RGB(0, 255, 0));
    }
    else
    {
        safeZoneText = L"전투 구역";
        SetTextColor(hdc, RGB(255, 0, 0));
    }

    RECT safeZoneRect = { _pos.x, _pos.y + _size.y, _pos.x + _size.x, _pos.y + _size.y + 20 };
    DrawTextW(hdc, safeZoneText.c_str(), -1, &safeZoneRect, DT_LEFT | DT_VCENTER | DT_WORDBREAK);

    Super::Render(hdc);
}