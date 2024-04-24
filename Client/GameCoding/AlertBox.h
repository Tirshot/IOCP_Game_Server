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

	void SetPos(Vec2 pos) { _pos = { pos.x - (int)_size.x / 2, pos.y - (int)_size.y / 2 }; }

private:
	void OnClickAcceptButton();
	void OnClickDenyButton();

public:
	void SetIcon(wstring wstr);
	void SetText(wstring wstr);

private:
	class Sprite* _background = nullptr;
	class Sprite* _icon = nullptr;
	RECT _rect = {};
};

