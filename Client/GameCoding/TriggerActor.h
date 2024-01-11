#pragma once
#include "GameObject.h"
class UI;
class TriggerActor : public GameObject
{	// ��ġ�� DevScene���� ProtoBuf ���
	using Super = Actor;
public:
	TriggerActor();
	virtual ~TriggerActor();

	virtual void BeginPlay();
	virtual void Tick();

	void PressSpaceInteract(UI* ui);
	void PressSpaceInteract();

	// ���� �Ǻ�
	bool IsTouched();

protected:
	bool _visiblity = false;
	float _exptime = 0.f; // Exposure time
};

