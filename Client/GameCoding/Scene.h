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
	vector<UI*> GetUIs() { return _uis; }
	vector<UI*>& GetUIRef() { return _uis; }


	template<typename T>
	void SetChildVisible(bool visible)
	{
		for (auto& ui : _uis)
		{
			T* child = dynamic_cast<T*>(ui);
			if (child)
			{
				child->SetVisible(visible);
				continue;
			}
		}
	}

	template<typename T>
	T* FindUI(const std::vector<UI*>& _uis)
	{
		for (auto& item : _uis)
		{
			T* foundItem = dynamic_cast<T*>(item);
			if (foundItem)
				return foundItem;
		}
		return nullptr;
	}

	vector<UI*> GetVisibleUIs();

public:
	vector<Actor*> _actors[LAYER_MAXCOUNT];
	vector<UI*> _uis;
	vector<UI*> _visibleUIs;

private:
	bool _isPaused = false;
};

