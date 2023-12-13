#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void TimeManager::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount)); // CPU 클럭

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime >= 1.f) // 1초가 증가했다면
	{
		// 초당 프레임 수 = 프레임 수 / 시간
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}

}
