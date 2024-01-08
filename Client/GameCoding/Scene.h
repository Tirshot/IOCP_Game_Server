#pragma once

class Actor;
class UI;
class Creature;
class Projectile;
class Player;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void AddActor(Actor* actor);
	virtual void RemoveActor(Actor* actor);

	virtual void AddUI(UI* ui);
	virtual void RemoveUI(UI* ui);

	bool GetPauseState() { return _isPaused; }
	void SetPauseState(bool pause) { _isPaused = pause; }

	Creature* GetCreatureAt(Vec2Int cellPos);
	Player* GetPlayerByID(uint64 objectId);

public:
	vector<Actor*> _actors[LAYER_MAXCOUNT];
	vector<UI*> _uis;

private:
	bool _isPaused = false;
};

