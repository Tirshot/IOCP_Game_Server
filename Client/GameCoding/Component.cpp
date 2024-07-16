#include "pch.h"
#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
	_owner = nullptr;
}

void Component::BeginPlay()
{
}

void Component::TickComponent()
{
}

void Component::Render(HDC hdc)
{
}
