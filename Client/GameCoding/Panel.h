#pragma once
#include "UI.h"

class Panel : public UI
{
	using Super = UI;

public:
	Panel();
	virtual ~Panel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	void AddChild(UI* ui);
	bool RemoveChild(UI* ui);

	auto& GetChildren() { return _children; }

protected:
	vector<UI*> _children;
};

