#pragma once
#include "GameObject.h"
class UI;
class MyPlayer;
class DevScene;
class TriggerActor : public GameObject
{	// ��ġ�� DevScene���� ProtoBuf ���
	using Super = Actor;
public:
	TriggerActor();
	virtual ~TriggerActor();

	virtual void BeginPlay();
	virtual void Tick();

	void PressSpaceInteract(UI* ui);
	void TouchInteract(UI* ui);

	// ���� �Ǻ�
	bool IsTouched();

	void FadeOut();

	void SetExpTime(float time) { _exptime = time; }

protected:
	bool _visiblity = false;
	float _exptime = 0.f; // Exposure time
	bool _isTouched = false;
};

