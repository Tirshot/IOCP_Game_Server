#pragma once

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void BeginPlay() {};
	virtual void Update() {};
	virtual void Render(HDC hdc) {};

};

