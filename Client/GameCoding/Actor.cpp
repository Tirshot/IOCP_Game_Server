#include "pch.h"
#include "Actor.h"
#include "Component.h"
#include "Collider.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::BeginPlay()
{
	for (Component* component : _components)
		component->BeginPlay();
}

void Actor::Tick()
{
	for (Component* component : _components)
		component->TickComponent();
}

void Actor::Render(HDC hdc)
{
	for (Component* component : _components)
		component->Render(hdc);
}

void Actor::AddComponent(Component* component)
{
	if (component == nullptr)
		return;

	component->SetOwner(this);
	_components.push_back(component);
}

void Actor::RemoveComponent(Component* component)
{
	auto findIt = std::find(_components.begin(), _components.end(), component);
	if (findIt == _components.end())
		return;

	_components.erase(findIt);
}
