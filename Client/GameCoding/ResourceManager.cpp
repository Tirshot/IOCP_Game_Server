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
	Clear();
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
	for (auto& item : _textures)
		SAFE_DELETE(item.second);

	_textures.clear();

	for (auto& item : _sprites)
		SAFE_DELETE(item.second);

	_sprites.clear();

	for (auto& item : _flipbooks)
		SAFE_DELETE(item.second);

	_flipbooks.clear();

	for (auto& item : _tilemaps)
		SAFE_DELETE(item.second);

	_tilemaps.clear();

	for (auto& item : _sounds)
		SAFE_DELETE(item.second);

	_sounds.clear();
}

Texture* ResourceManager::LoadTexture(const wstring& key, const wstring& path, uint32 transparent)
{
	// ã�� �ؽ��İ� �ִٸ� ��ȯ
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	// �ؽ��İ� ���ٸ� �⺻ ��ο� �Ű� ���� path�� ��θ� ��ħ
	fs::path fullPath = _resourcePath / path;

	Texture* texture = new Texture();
	texture->LoadBmp(_hwnd, fullPath.c_str());
	texture->SetTransparent(transparent);

	// �ؽ��ĸ� ���� �� ��ȯ
	_textures[key] = texture;

	return texture;
}

Sprite* ResourceManager::CreateSprite(const wstring& key, Texture* texture, int32 x, int32 y, int32 cx, int32 cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	// size�� 0, 0�� �����ϸ� �״�� ���
	if (cx == 0)
		cx = texture->GetSize().x;

	if (cy == 0)
		cy = texture->GetSize().y;

	Sprite* sprite = new Sprite(texture, x, y, cx, cy);
	_sprites[key] = sprite;

	return sprite;
}

Flipbook* ResourceManager::CreateFlipbook(const wstring& key)
{
	// �̹� �ø��� �ؽøʿ� ���� �Ѵٸ� �ش� �ø����� ��ȯ
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	// ���ٸ� ���� �����ϰ� ��ȯ
	Flipbook* fb = new Flipbook();
	_flipbooks[key] = fb;

	return fb;
}

Tilemap* ResourceManager::CreateTilemap(const wstring& key)
{
	// �̹� Ÿ�ϸ� �ؽøʿ� ���� �Ѵٸ� �ش� Ÿ�ϸ��� ��ȯ
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	// ���ٸ� ���� �����ϰ� ��ȯ
	Tilemap* tm = new Tilemap();
	_tilemaps[key] = tm;

	return tm;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tm = GetTilemap(key);
	// ���ҽ� ������ ��ġ�� path ��θ� �̾����
	fs::path fullPath = _resourcePath / path;
	tm->SaveFile(fullPath);
}

Tilemap* ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tm = nullptr;
	
	// key�� �ش��ϴ� Ÿ�ϸ��� ã�� ���ߴٸ� ���� ����
	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = new Tilemap();

	tm = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tm->LoadFile(fullPath);

	return tm;
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path, SoundType type)
{
	// �̹� �����ϸ� ã�Ƽ� ��ȯ
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	// ������ ���� ����
	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
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

	// ù ��° ���� �ǳʶٱ� ���� �� �� �� getline ȣ��
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
			// �����ݷ��� �ٹٲ� ���ڷ� ��ü
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

