#pragma once
#include "UI.h"
class Sprite;
class SliderBar :  public UI
{
	using Super = UI;

public:
	SliderBar();
	virtual ~SliderBar();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

public:
	void SetMaximumValue(float max) { _maximum = max; }
	void SetMinimumValue(float min) { _minimum = min; }
	float GetValue() { return _value; }

	void SetName(wstring wstr) { _name = wstr; }
	wstring GetName() { return _name; }

	template<typename T>
	void AddOnSlideDelegate(T* owner, void(T::* func)())
	{
		_callbackFunc = [owner, func]()
			{
				(owner->*func)();
			};
	}
	std::function<void(void)>_callbackFunc = nullptr;

private:
	Sprite* _background = nullptr;
	Sprite* _backgroundFill = nullptr;

	float _percentage = 0.f;
	float _maximum = 100;
	float _minimum = 0;

	float _value = 50.f;
	wstring _name = L"";
};

