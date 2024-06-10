#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "UI.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ChatManager.h"
#include "ResourceManager.h"
#include "Creature.h"
#include "Panel.h"
#include "Chat.h"
#include "Player.h"
#include "Inventory.h"
#include "ShopUI.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	GET_SINGLE(ResourceManager)->Clear();
}

void Scene::Init()
{
	_sceneChanged = false;

	//
	for (const vector<shared_ptr<Actor>>& actors : _actors)
		for (shared_ptr<Actor> actor : actors)
			actor->BeginPlay();

	for (auto& ui : _uis)
		ui->BeginPlay();
}

void Scene::Update()
{
	if (_isPaused)
		return;

	for (const vector<shared_ptr<Actor>> actors : _actors)
		for (shared_ptr<Actor> actor : actors)
			actor->Tick();

	for (auto& ui : _uis)
	{
		if (_isPaused)
			return;

		if (ui == nullptr)
			return;

		if (_sceneChanged)
			return;

		bool visiblity = ui->GetVisible();

		if (visiblity)
			ui->Tick();
	}

	shared_ptr<Chat> chat = FindUI<Chat>(_uis);
	if(chat)
		chat->Tick();

	shared_ptr<Inventory> inven = FindUI<Inventory>(_uis);
	if (inven)
		inven->Tick();
}

void Scene::Render(HDC hdc)
{
	// y 좌표가 더 큰쪽이 위로 오도록 설정
	vector<shared_ptr<Actor>>& actors = _actors[LAYER_OBJECT];
	std::sort(actors.begin(), actors.end(), [=](shared_ptr<Actor> a, shared_ptr<Actor> b)
		{
			return a->GetPos().y < b->GetPos().y;
		});

	// UIID가 더 높은 쪽이 위로 오도록 설정
	std::sort(_uis.begin(), _uis.end(), [=](auto& a, auto& b)
		{
			return a->GetUIID() < b->GetUIID();
		});

	for (const vector<shared_ptr<Actor>>& actors : _actors)
		for (shared_ptr<Actor> actor : actors)
			actor->Render(hdc);

	for (auto& ui : _uis)
		if (ui->GetVisible())
			ui->Render(hdc);
}

void Scene::AddActor(shared_ptr<Actor> actor)
{
	if (actor == nullptr)
		return;

	_actors[actor->GetLayer()].push_back(actor);
}

void Scene::RemoveActor(shared_ptr<Actor> actor)
{
	if (actor == nullptr)
		return;

	vector<shared_ptr<Actor>>& v = _actors[actor->GetLayer()];

	v.erase(std::remove(v.begin(), v.end(), actor), v.end());
}

void Scene::AddUI(shared_ptr<UI> ui)
{
	if (ui == nullptr)
		return;

	_uis.push_back(ui);
}

void Scene::RemoveUI(shared_ptr<UI> ui)
{
	if (ui == nullptr)
		return;

	vector<shared_ptr<UI>>& v = _uis;

	v.erase(std::remove(v.begin(), v.end(), ui), v.end());
}

shared_ptr<Creature> Scene::GetCreatureAt(Vec2Int cellPos)
{
	for (auto& actor : _actors[LAYER_OBJECT])
	{
		auto creature = dynamic_pointer_cast<Creature>(actor);
		if (creature && creature->GetCellPos() == cellPos)
			return creature;
	}

	return nullptr;
}

shared_ptr<Player> Scene::GetPlayerByID(uint64 objectId)
{
	for (auto& actor : _actors)
	{
		for (auto& creature : actor)
		{
			auto player = dynamic_pointer_cast<Player>(creature);

			if (!player)
				continue;

			if (player->info.objectid() == objectId)
				return player ? player : 0;
		}
	}
}

vector<shared_ptr<UI>> Scene::GetVisibleUIs()
{
	vector<shared_ptr<UI>> visibleUIs;

	auto _uis = GetUIs();

	for (auto& ui : _uis)
	{
		if (ui->GetVisible())
			visibleUIs.push_back(ui);
	}
	return visibleUIs;
}
