#include "pch.h"
#include "Tilemap.h"
#include <fstream>
#include <iostream>

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::LoadFile(const wstring& path)
{
	// C++ ��Ÿ��
	{
		wifstream ifs;
		ifs.open(path);

		// _mapSize.x �����ϱ�
		ifs >> _mapSize.x;
		ifs >> _mapSize.y;

		// _tiles �ʱ�ȭ
		SetMapSize(_mapSize);

		// _tiles ���� ���� �迭 �����ϱ�
		for (int32 y = 0; y < _mapSize.y; y++)
		{
			wstring line;
			ifs >> line;

			for (int32 x = 0; x < _mapSize.x; x++)
			{
				_tiles[y][x].value = line[x] - L'0';
			}
		}
		ifs.close();
	}
}

Tile* Tilemap::GetTileAt(Vec2Int pos)
{
	if (pos.x < 0 || pos.x >= _mapSize.x || pos.y < 0 || pos.y >= _mapSize.y)
		return nullptr;

	return &_tiles[pos.y][pos.x];
}

void Tilemap::SetMapSize(Vec2Int size)
{
	_mapSize = size;

	// ���� �ʱ�ȭ
	_tiles = vector<vector<Tile>>(size.y, vector<Tile>(size.x));

	for (int32 y = 0; y < size.y; y++)
	{
		for (int32 x = 0; x < size.x; x++)
		{
			_tiles[y][x] = Tile{ 0 };
		}
	}
}

void Tilemap::SetTileSize(int32 size)
{
	_tileSize = size;
}
