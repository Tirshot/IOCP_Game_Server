#pragma once

class ResourceBase;
class Texture;
class Sprite;
class Flipbook;
class Tilemap;
class Sound;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);

	~ResourceManager();

public:
	void Init(HWND hwnd, fs::path resourcePath);
	void Clear();

	const fs::path& GetResourcePath() { return _resourcePath; }

	shared_ptr<Texture> GetTexture(const wstring& key) { return _textures[key]; }
	shared_ptr<Texture> LoadTexture(const wstring& key, const wstring& path, uint32 transparent = RGB(255, 0, 255));

	shared_ptr<Sprite> GetSprite(const wstring& key) { return _sprites[key]; }
	shared_ptr<Sprite> CreateSprite(const wstring& key, shared_ptr<Texture> texture, int32 x = 0, int32 y = 0, int32 cx = 0, int32 cy = 0);

	shared_ptr<Flipbook> GetFlipbook(const wstring& key) { return _flipbooks[key]; }
	shared_ptr<Flipbook> CreateFlipbook(const wstring& key);

	shared_ptr<Tilemap> GetTilemap(const wstring& key) { return _tilemaps[key]; }
	shared_ptr<Tilemap> CreateTilemap(const wstring& key);
	void SaveTilemap(const wstring& key, const wstring& path);
	shared_ptr<Tilemap> LoadTilemap(const wstring& key, const wstring& path);

	Sound* GetSound(const wstring& key) { return _sounds[key]; }
	unordered_map<wstring, Sound*> GetSounds() { return _sounds; }
	Sound* LoadSound(const wstring& key, const wstring& path, SoundType type);

	vector<vector<wstring>> GetDataFromCSV(const string& filename);
	vector<vector<wstring>> GetItemTable() { return _itemTable; }
	vector<vector<wstring>> GetQuestTable() { return _questTable; }

	unordered_map<wstring, shared_ptr<Sprite>> GetSprites() { return _sprites; }

	void DrawImage(HDC hdc, shared_ptr<Sprite> sprite);

private:
	HWND _hwnd = {};
	fs::path _resourcePath;

	unordered_map<wstring, shared_ptr<Texture>> _textures;
	unordered_map<wstring, shared_ptr<Sprite>> _sprites;
	unordered_map<wstring, shared_ptr<Flipbook>> _flipbooks;
	unordered_map<wstring, shared_ptr<Tilemap>> _tilemaps;
	unordered_map<wstring, Sound*> _sounds;
	vector<vector<wstring>> _itemTable;
	vector<vector<wstring>> _questTable;
};

