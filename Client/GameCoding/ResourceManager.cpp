#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Tilemap.h"
#include "Sound.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>

ResourceManager::~ResourceManager()
{
	
}

void ResourceManager::Init(HWND hwnd, fs::path resourcePath)
{
	_hwnd = hwnd;
	_resourcePath = resourcePath;
	_itemTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Client\\Resources\\Table\\ItemTable.csv");
	_questTable = GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Client\\Resources\\Table\\QuestScript.csv");
}

void ResourceManager::Clear()
{
	_textures.clear();
	_sprites.clear();
	_flipbooks.clear();
	_tilemaps.clear();
	_sounds.clear();
}

shared_ptr<Texture> ResourceManager::LoadTexture(const wstring& key, const wstring& path, uint32 transparent)
{
	// 찾는 텍스쳐가 있다면 반환
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	// 텍스쳐가 없다면 기본 경로에 매개 변수 path의 경로를 합침
	fs::path fullPath = _resourcePath / path;

	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->LoadBmp(_hwnd, fullPath.c_str());
	texture->SetTransparent(transparent);

	// 텍스쳐를 저장 후 반환
	_textures[key] = texture;

	return texture;
}

shared_ptr<Sprite> ResourceManager::CreateSprite(const wstring& key, shared_ptr<Texture> texture, int32 x, int32 y, int32 cx, int32 cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	// size를 0, 0로 지정하면 그대로 사용
	if (cx == 0)
		cx = texture->GetSize().x;

	if (cy == 0)
		cy = texture->GetSize().y;

	shared_ptr<Sprite> sprite = make_shared<Sprite>(texture, x, y, cx, cy);
	_sprites[key] = sprite;

	return sprite;
}

shared_ptr<Flipbook> ResourceManager::CreateFlipbook(const wstring& key)
{
	// 이미 플립북 해시맵에 존재 한다면 해당 플립북을 반환
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	// 없다면 새로 저장하고 반환
	shared_ptr<Flipbook> fb = make_shared<Flipbook>();
	_flipbooks[key] = fb;

	return fb;
}

shared_ptr<Tilemap> ResourceManager::CreateTilemap(const wstring& key)
{
	// 이미 타일맵 해시맵에 존재 한다면 해당 타일맵을 반환
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	// 없다면 새로 저장하고 반환
	shared_ptr<Tilemap> tm = make_shared<Tilemap>();
	_tilemaps[key] = tm;

	return tm;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	shared_ptr<Tilemap> tm = GetTilemap(key);
	// 리소스 폴더의 위치에 path 경로를 이어붙임
	fs::path fullPath = _resourcePath / path;
	tm->SaveFile(fullPath);
}

shared_ptr<Tilemap> ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	shared_ptr<Tilemap> tm = nullptr;
	
	// key에 해당하는 타일맵을 찾지 못했다면 새로 생성
	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = make_shared<Tilemap>();

	tm = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tm->LoadFile(fullPath);

	return tm;
}

shared_ptr<Sound> ResourceManager::LoadSound(const wstring& key, const wstring& path, SoundType type)
{
	// 이미 존재하면 찾아서 반환
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	// 없으면 새로 생성
	fs::path fullPath = _resourcePath / path;

	shared_ptr<Sound> sound = make_shared<Sound>();
	sound->LoadWave(fullPath);
	sound->SetType(type);
	_sounds[key] = sound;

	return sound;
}

vector<vector<wstring>> ResourceManager::GetDataFromCSV(const string& filename)
{
	vector<vector<wstring>> data;
	locale::global(locale("korean"));

	wifstream file(filename);
	if (!file.is_open())
	{
		return data;
	}

	// 첫 번째 행을 건너뛰기 위해 한 번 더 getline 호출
	wstring header;
	getline(file, header);

	wstring line;
	while (getline(file, line))
	{
		vector<wstring> row;
		wstringstream ss(line);
		wstring cell;

		while (getline(ss, cell, L','))
		{
			// 세미콜론을 줄바꿈 문자로 대체
			size_t pos = cell.find(L';');

			while (pos != wstring::npos)
			{
				cell.replace(pos, 1, L"\n");
				pos = cell.find(L';', pos + 1);
			}
			row.push_back(cell);
		}
		data.push_back(row);
	}

	file.close();

	return data;
}

