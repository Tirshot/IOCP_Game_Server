#pragma once
#include "GameObject.h"
class UI;
class TriggerActor : public GameObject
{	// 배치는 DevScene에서 ProtoBuf 사용
	using Super = Actor;
public:
	TriggerActor();
	virtual ~TriggerActor();

	virtual void BeginPlay();
	virtual void Tick();

	void PressSpaceInteract(UI* ui);
	void PressSpaceInteract();

	// 접촉 판별
	bool IsTouched();

protected:
	bool _visiblity = false;
	float _exptime = 0.f; // Exposure time
};

