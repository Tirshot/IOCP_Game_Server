#include "pch.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Sound.h"

SoundManager::~SoundManager()
{
	if (_soundDevice)
		_soundDevice->Release();
}

void SoundManager::Init(HWND hwnd)
{
	// ���� ����̽� ����
	if (FAILED(::DirectSoundCreate(NULL, &_soundDevice, NULL)))
	{
		::MessageBox(NULL, L"�������̽���������", L"SYSTEM ERROR", MB_OK);
		return;
	}

	// ���� ����̽� �������� ����
	if (FAILED(_soundDevice->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		::MessageBox(NULL, L"�������̽� �������� ����", L"SYSTEM ERROR", MB_OK);
		return;
	}
}

void SoundManager::Play(const wstring& key, bool loop /*= false*/)
{
	auto sound = GET_SINGLE(ResourceManager)->GetSound(key);
	if (sound == nullptr)
		return;

	sound->Play(loop);

	UpdateVolume();
}

void SoundManager::SetVolume(SoundType type, long vol)
{
	auto sounds = GET_SINGLE(ResourceManager)->GetSounds();

	if (sounds.empty() == false)
	{
		for (auto& sonic : sounds)
		{
			auto sound = sonic.second;

			if (sound == nullptr)
				continue;

			SoundType soundType = sound->GetType();

			if (soundType == type)
				sound->SetVolume(vol);
		}
	}
}

void SoundManager::SetMasterVolume(float volume)
{
	_masterVolume = volume;
	UpdateVolume();
}

void SoundManager::SetBGMVolume(float volume)
{
	_bgmVolume = volume;
	UpdateVolume();
}

void SoundManager::SetSFXVolume(float volume)
{
	_sfxVolume = volume;
	UpdateVolume();
}

void SoundManager::SetUIVolume(float volume)
{
	_uiVolume = volume;
	UpdateVolume();
}

void SoundManager::UpdateVolume()
{
	float finalBGMVolume = _masterVolume * _bgmVolume;
	float finalSFXVolume = _masterVolume * _sfxVolume;
	float finalUIVolume = _masterVolume * _uiVolume;

	// �� �纸��
	long bgmVolumeDB = VolumeToDB(finalBGMVolume);
	long sfxVolumeDB = VolumeToDB(finalSFXVolume);
	long uiVolumeDB = VolumeToDB(finalUIVolume);

	// �� ī�װ��� DirectSound ���ۿ� ���� SetVolume ȣ��
	SetVolume(SoundType::BGM, bgmVolumeDB);
	SetVolume(SoundType::Effect, sfxVolumeDB);
	SetVolume(SoundType::UI, uiVolumeDB);
}

long SoundManager::VolumeToDB(float volume)
{
	if (volume <= 0.0f)
	{
		return -10000;
	}
	else if (volume >= 1.0f)
	{
		return 0;
	}

	return (20.0f * log10(volume) * 100.0f);
}