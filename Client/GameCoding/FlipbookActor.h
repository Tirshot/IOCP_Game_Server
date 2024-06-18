#pragma once
#include "Actor.h"

class Flipbook;
class FlipbookActor : public Actor
{
	using Super = Actor;
public:
	FlipbookActor();
	virtual ~FlipbookActor() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetFlipbook(shared_ptr<Flipbook> flipbook);
	void Reset();

	bool IsAnimationEnded();

protected:
	shared_ptr<Flipbook> _flipbook = nullptr;
	// 해당 플립북을 어디까지 재생했는지
	float _sumTime = 0.f;
	int32 _idx = 0;
};

