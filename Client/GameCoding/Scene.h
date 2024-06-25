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

	virtual void AddActor(shared_ptr<Actor> actor);
	virtual void RemoveActor(shared_ptr<Actor> actor);

	virtual void AddUI(shared_ptr<UI> ui);
	virtual void RemoveUI(shared_ptr<UI> ui);

	bool GetPauseState() { return _isPaused; }
	void SetPauseState(bool pause) { _isPaused = pause; }
	bool isInitialized() { return _isInitialized; }
	void SetInitialized(bool init) { _isInitialized = init; }

	shared_ptr<Creature> GetCreatureAt(Vec2Int cellPos);
	shared_ptr<Player> GetPlayerByID(uint64 objectId);
	vector<shared_ptr<UI>> GetUIs() { return _uis; }
	vector<shared_ptr<UI>>& GetUIRef() { return _uis; }

	vector<shared_ptr<Actor>> GetActors() { return _actors[LAYER_OBJECT]; }

	template<typename T>
	void SetChildVisible(bool visible)
	{
		for (auto& ui : _uis)
		{
			shared_ptr<T> child = dynamic_pointer_cast<T>(ui);
			if (child)
			{
				child->SetVisible(visible);
				continue;
			}
		}
	}

	template<typename T>
	shared_ptr<T> FindUI(const std::vector<shared_ptr<UI>>& _uis)
	{
		for (auto& item : _uis)
		{
			shared_ptr<T> foundItem = dynamic_pointer_cast<T>(item);
			if (foundItem)
				return foundItem;
		}
		return nullptr;
	}

	vector<shared_ptr<UI>> GetVisibleUIs();

public:
	vector<shared_ptr<Actor>> _actors[LAYER_MAXCOUNT];
	vector<shared_ptr<UI>> _uis;
	vector<shared_ptr<UI>> _visibleUIs;

protected:
	bool _isPaused = false;
	bool _isInitialized = false;
	bool _sceneChanged = false;
};

