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
	// 찾는 텍스쳐가 있다면 반환
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	// 텍스쳐가 없다면 기본 경로에 매개 변수 path의 경로를 합침
	fs::path fullPath = _resourcePath / path;

	Texture* texture = new Texture();
	texture->LoadBmp(_hwnd, fullPath.c_str());
	texture->SetTransparent(transparent);

	// 텍스쳐를 저장 후 반환
	_textures[key] = texture;

	return texture;
}

Sprite* ResourceManager::CreateSprite(const wstring& key, Texture* texture, int32 x, int32 y, int32 cx, int32 cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	// size를 0, 0로 지정하면 그대로 사용
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
	// 이미 플립북 해시맵에 존재 한다면 해당 플립북을 반환
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	// 없다면 새로 저장하고 반환
	Flipbook* fb = new Flipbook();
	_flipbooks[key] = fb;

	return fb;
}

Tilemap* ResourceManager::CreateTilemap(const wstring& key)
{
	// 이미 타일맵 해시맵에 존재 한다면 해당 타일맵을 반환
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	// 없다면 새로 저장하고 반환
	Tilemap* tm = new Tilemap();
	_tilemaps[key] = tm;

	return tm;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tm = GetTilemap(key);
	// 리소스 폴더의 위치에 path 경로를 이어붙임
	fs::path fullPath = _resourcePath / path;
	tm->SaveFile(fullPath);
}

Tilemap* ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tm = nullptr;
	
	// key에 해당하는 타일맵을 찾지 못했다면 새로 생성
	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = new Tilemap();

	tm = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tm->LoadFile(fullPath);

	return tm;
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	// 이미 존재하면 찾아서 반환
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	// 없으면 새로 생성
	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}

