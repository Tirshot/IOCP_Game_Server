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
	// ã�� �ؽ��İ� �ִٸ� ��ȯ
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	// �ؽ��İ� ���ٸ� �⺻ ��ο� �Ű� ���� path�� ��θ� ��ħ
	fs::path fullPath = _resourcePath / path;

	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->LoadBmp(_hwnd, fullPath.c_str());
	texture->SetTransparent(transparent);

	// �ؽ��ĸ� ���� �� ��ȯ
	_textures[key] = texture;

	return texture;
}

shared_ptr<Sprite> ResourceManager::CreateSprite(const wstring& key, shared_ptr<Texture> texture, int32 x, int32 y, int32 cx, int32 cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	// size�� 0, 0�� �����ϸ� �״�� ���
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
	// �̹� �ø��� �ؽøʿ� ���� �Ѵٸ� �ش� �ø����� ��ȯ
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	// ���ٸ� ���� �����ϰ� ��ȯ
	shared_ptr<Flipbook> fb = make_shared<Flipbook>();
	_flipbooks[key] = fb;

	return fb;
}

shared_ptr<Tilemap> ResourceManager::CreateTilemap(const wstring& key)
{
	// �̹� Ÿ�ϸ� �ؽøʿ� ���� �Ѵٸ� �ش� Ÿ�ϸ��� ��ȯ
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	// ���ٸ� ���� �����ϰ� ��ȯ
	shared_ptr<Tilemap> tm = make_shared<Tilemap>();
	_tilemaps[key] = tm;

	return tm;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	shared_ptr<Tilemap> tm = GetTilemap(key);
	// ���ҽ� ������ ��ġ�� path ��θ� �̾����
	fs::path fullPath = _resourcePath / path;
	tm->SaveFile(fullPath);
}

shared_ptr<Tilemap> ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	shared_ptr<Tilemap> tm = nullptr;
	
	// key�� �ش��ϴ� Ÿ�ϸ��� ã�� ���ߴٸ� ���� ����
	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = make_shared<Tilemap>();

	tm = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tm->LoadFile(fullPath);

	return tm;
}

shared_ptr<Sound> ResourceManager::LoadSound(const wstring& key, const wstring& path, SoundType type)
{
	// �̹� �����ϸ� ã�Ƽ� ��ȯ
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	// ������ ���� ����
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

