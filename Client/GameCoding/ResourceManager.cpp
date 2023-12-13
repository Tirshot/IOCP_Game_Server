#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Tilemap.h"
#include "Sound.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Init(HWND hwnd, fs::path resourcePath)
{
	_hwnd = hwnd;
	_resourcePath = resourcePath;
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

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	// �̹� �����ϸ� ã�Ƽ� ��ȯ
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	// ������ ���� ����
	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}
