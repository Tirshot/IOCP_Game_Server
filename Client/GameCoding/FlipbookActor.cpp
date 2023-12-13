#include "pch.h"
#include "FlipbookActor.h"
#include "Flipbook.h"
#include "Texture.h"
#include "Sprite.h"
#include "TimeManager.h"
#include "SceneManager.h"

FlipbookActor::FlipbookActor()
{
}

FlipbookActor::~FlipbookActor()
{
}

void FlipbookActor::BeginPlay()
{
	Super::BeginPlay();

}

void FlipbookActor::Tick()
{
	Super::Tick();

	if (_flipbook == nullptr)
		return;

	const FlipbookInfo& info = _flipbook->GetInfo();

	// �ݺ��� �ʿ���� �ִϸ��̼��̰� ���� �����ߴٸ� ����
	if (info.loop == false && _idx == info.end)
		return;

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_sumTime += deltaTime;

	// �ִϸ��̼��� �� ���������� �Ǿ��ִ°�
	int32 frameCount = (info.end - info.start + 1);

	// �� ������ �� ���� ����ð�
	float delta = info.duration / frameCount;

	if (_sumTime >= delta)
	{
		_sumTime = 0.f;
		// �� ������ ���� �ʰ��ϸ� �ٽ� ù��° �׸����� ����ϱ�
		_idx = (_idx + 1) % frameCount;
	}
}

void FlipbookActor::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_flipbook == nullptr)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	const FlipbookInfo& info = _flipbook->GetInfo();

	::TransparentBlt(hdc,
		(int32)_pos.x - info.size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - info.size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		info.size.x,
		info.size.y,
		info.texture->GetDC(),
		(info.start + _idx) * info.size.x,
		info.line * info.size.y,
		info.size.x,
		info.size.y,
		info.texture->GetTransparent());
}

void FlipbookActor::SetFlipbook(Flipbook* flipbook)
{
	// ��û�� ���� ������ �ø����̶�� (ù��° �׸����� �Ѿ�� �ʴ� ���)
	if (flipbook && _flipbook == flipbook)
		return;


	_flipbook = flipbook;
	Reset();
}

void FlipbookActor::Reset()
{
	_sumTime = 0.f;
	_idx = 0;
}

bool FlipbookActor::IsAnimationEnded()
{
	if (_flipbook == nullptr)
		return true;

	// loop�� �����ִ��� Ȯ��
	const FlipbookInfo& info = _flipbook->GetInfo();
	if (info.loop == false && _idx == info.end)
		return true;

	return false;
}
