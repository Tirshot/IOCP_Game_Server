#include "pch.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

TilemapActor::TilemapActor()
{
}

TilemapActor::~TilemapActor()
{
}

void TilemapActor::BeginPlay()
{
	Super::BeginPlay();
}

void TilemapActor::Tick()
{
	Super::Tick();

	// 타일맵 수정
	TilePicking();
}

void TilemapActor::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_tilemap == nullptr)
		return;

	if (_showDebug == false)
		return;

	// 타일맵을 화면에 그림
	const Vec2Int mapSize = _tilemap->GetMapSize();
	const int32 tileSize = _tilemap->GetTileSize();

	vector<vector<Tile>>& tiles = _tilemap->GetTiles();

	Sprite* spriteO = GET_SINGLE(ResourceManager)->GetSprite(L"TileO");
	Sprite* spriteX = GET_SINGLE(ResourceManager)->GetSprite(L"TileX");
	Vec2Int size = spriteO->GetSize();
	// 타일맵도 움직이게 처리
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	// 컬링 ; 보이지 않는 영역의 물체를 렌더링 하지 않음
	int32 leftX = ((int32)cameraPos.x - GWinSizeX / 2);
	int32 leftY = ((int32)cameraPos.y - GWinSizeY / 2);
	int32 rightX = ((int32)cameraPos.x + GWinSizeX / 2);
	int32 rightY = ((int32)cameraPos.y + GWinSizeY / 2);

	// 몇번째 칸부터 시작인가
	int32 startX = (int32)(leftX - _pos.x) / (int32)TILE_SIZEX;
	int32 startY = (int32)(leftY - _pos.y) / (int32)TILE_SIZEY;
	int32 endX = (int32)(rightX - _pos.x) / (int32)TILE_SIZEX;
	int32 endY = (int32)(rightY - _pos.y) / (int32)TILE_SIZEY;


	for (int32 y = startY; y <= endY; y++)
	{
		for (int32 x = startX; x <= endX; x++)
		{
			if (x < 0 || x >= mapSize.x)
				continue;

			if (y < 0 || y >= mapSize.y)
				continue;

			switch (tiles[y][x].value)
			{
			case 0:
				// 타일맵의 좌상단이 타일맵 액터의 중심에 위치
				::TransparentBlt(hdc,
					_pos.x + x * (int32)TILE_SIZEX - ((int32)cameraPos.x - GWinSizeX / 2),
					_pos.y + y * (int32)TILE_SIZEX - ((int32)cameraPos.y - GWinSizeY / 2),
					TILE_SIZEX,
					TILE_SIZEY,
					spriteO->GetDC(),
					spriteO->GetPos().x,
					spriteO->GetPos().y,
					TILE_SIZEX,
					TILE_SIZEY,
					spriteO->GetTransparent()
				);
				break;

			case 1:
				// 타일맵의 좌상단이 타일맵 액터의 중심에 위치
				::TransparentBlt(hdc,
					_pos.x + x * (int32)TILE_SIZEX - ((int32)cameraPos.x - GWinSizeX / 2),
					_pos.y + y * (int32)TILE_SIZEX - ((int32)cameraPos.y - GWinSizeY / 2),
					TILE_SIZEX,
					TILE_SIZEY,
					spriteX->GetDC(),
					spriteX->GetPos().x,
					spriteX->GetPos().y,
					TILE_SIZEX,
					TILE_SIZEY,
					spriteX->GetTransparent()
				);
				break;
			}
		}
	}
}

void TilemapActor::TilePicking()
{
	// 인 게임 좌표
	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	int32 screenX = cameraPos.x - GWinSizeX / 2;
	int32 screenY = cameraPos.y - GWinSizeY / 2;

	// 800 * 600 내에서 마우스의 좌표
	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
	int32 posX = mousePos.x + screenX;
	int32 posY = mousePos.y + screenY;

	int32 x = posX / TILE_SIZEX;
	int32 y = posY / TILE_SIZEY;

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
	{
		Tile* tile = _tilemap->GetTileAt({ x,y });
		if (tile)
			tile->value = 1;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::RightMouse))
	{
		Tile* tile = _tilemap->GetTileAt({ x,y });
		if (tile)
			tile->value = 0;
	}
}
