#pragma once
#include "ResourceBase.h"

class Sound : public ResourceBase
{
public:
	Sound();
	virtual ~Sound();

	bool LoadWave(fs::path fullPath);
	void Play(bool loop = false);
	void Stop(bool reset = false);
	void SetVolume(long vol);
	void SetType(SoundType soundType) { _type = soundType; }
	void Clear();
	SoundType GetType() { return _type; }

private:
	virtual void LoadFile(const wstring& path) {}
	virtual void SaveFile(const wstring& path) {}

private:
	LPDIRECTSOUNDBUFFER _soundBuffer = nullptr;
	DSBUFFERDESC _bufferDesc = {};
	SoundType _type = SoundType::NONE;
};
