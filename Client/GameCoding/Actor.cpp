#include "pch.h"
#include "Actor.h"
#include "Component.h"
#include "Collider.h"

Actor::Actor()
{
}

Actor::~Actor()
{
	for (auto& component : _components)
		RemoveComponent(component);
}

void Actor::BeginPlay()
{
	for (shared_ptr<Component> component : _components)
		component->BeginPlay();
}

void Actor::Tick()
{
	for (shared_ptr<Component> component : _components)
		component->TickComponent();
}

void Actor::Render(HDC hdc)
{
	for (shared_ptr<Component> component : _components)
		component->Render(hdc);
}

void Actor::AddComponent(shared_ptr<Component> component)
{
	if (component == nullptr)
		return;

	component->SetOwner(this);
	_components.push_back(component);
}

void Actor::RemoveComponent(shared_ptr<Component> component)
{
	auto findIt = std::find(_components.begin(), _components.end(), component);
	if (findIt == _components.end())
		return;

	_components.erase(findIt);
}
