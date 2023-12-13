#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "SceneManager.h"
#include "Actor.h"

SphereCollider::SphereCollider() : Collider(ColliderType::Sphere)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::BeginPlay()
{
	Super::BeginPlay();

}

void SphereCollider::TickComponent()
{
	Super::TickComponent();

}

void SphereCollider::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_showDebug == false)
		return;

	Vec2 cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	Vec2 pos = GetOwner()->GetPos();
	pos.x -= (cameraPos.x - GWinSizeX / 2);
	pos.y -= (cameraPos.y - GWinSizeY / 2);

	HBRUSH myBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, myBrush);
	Utils::DrawCircle(hdc, pos, _radius);
	::SelectObject(hdc, oldBrush);
	::DeleteObject(myBrush);

}

bool SphereCollider::CheckCollision(Collider* other)
{
	if (Super::CheckCollision(other) == false)
		return false;

	switch (other->GetColliderType())
	{
	case ColliderType::Sphere:
		return CheckCollisionSphere2Sphere(this, static_cast<SphereCollider*>(other));
		
	case ColliderType::Box:
		return CheckCollisionSphere2Box(this, static_cast<BoxCollider*>(other));
	}
	return false;
}
