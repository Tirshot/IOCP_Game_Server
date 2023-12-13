#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "Actor.h"

void CollisionManager::Init()
{
	// Collision INIT
}

void CollisionManager::Update()
{
	vector<Collider*>& colliders = _colliders;

	// 충돌 판정
	for (int32 i = 0; i < colliders.size(); i++)
	{
		for (int32 j = i + 1; j < colliders.size(); j++)
		{
			Collider* src = colliders[i];
			Collider* dest = colliders[j];

			if (src->CheckCollision(dest))
			{
				// 처음 충돌 했을 때
				if (src->_collisionMap.contains(dest) == false)
				{
					src->GetOwner()->OnComponentBeginOverlap(src, dest);
					dest->GetOwner()->OnComponentBeginOverlap(dest, src);
					src->_collisionMap.insert(dest);
					dest->_collisionMap.insert(src);
				}
			}
			else
			{
				// 이전에 충돌한 후에 더 이상 충돌하지 않음
				if (src->_collisionMap.contains(dest))
				{
					src->GetOwner()->OnComponentEndOverlap(src, dest);
					dest->GetOwner()->OnComponentEndOverlap(dest, src);
					src->_collisionMap.erase(dest);
					dest->_collisionMap.erase(src);
				}
			}
		}
	}
}

void CollisionManager::AddCollider(Collider* collider)
{
	_colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider)
{
	auto it = std::remove(_colliders.begin(), _colliders.end(), collider);
	_colliders.erase(it, _colliders.end());
}
