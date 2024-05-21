#pragma once
#include "Panel.h"
class PopUp : public Panel
{
	using Super = Panel;

public:
	PopUp();
	virtual ~PopUp();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

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
	void SetVisible(bool visible);
};

