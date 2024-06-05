#pragma once

class Sound;
class SoundManager
{
	DECLARE_SINGLE(SoundManager);

	~SoundManager();

public:
	void Init(HWND hwnd);

	void Play(const wstring& key, bool loop = false);

	LPDIRECTSOUND GetSoundDevice() { return _soundDevice; }

	void SetVolume(SoundType type, long vol);

	// 0(-100dB) ~ 1(기본)
	void SetMasterVolume(float volume);
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume); 
	void SetUIVolume(float volume);

	void UpdateVolume();
	long VolumeToDB(float volume);

private:
	float _masterVolume = 1.f;
	float _bgmVolume = 1.f;
	float _sfxVolume = 1.f;
	float _uiVolume = 1.f;

private:
	LPDIRECTSOUND _soundDevice = nullptr; // 사운드 카드
};

