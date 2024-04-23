#pragma once
#include "Panel.h"
class AlertBox :  public Panel
{
	using Super = Panel;

public:
	AlertBox();
	virtual ~AlertBox();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	template<typename T>
	void AddParentDelegate(T* parent, void(T::* parentFunc)())
	{
		_parentCallback = [parent, parentFunc]()
			{
				(parent->*parentFunc)();
			};
	}
	std::function<void(void)>_parentCallback = nullptr;

private:
	void SetIcon(wstring wstr);
	void OnClickAcceptButton();

private:
	class Sprite* _background = nullptr;
	class Sprite* _icon = nullptr;
};

