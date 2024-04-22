#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "UI.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Creature.h"
#include "Panel.h"
#include "Chat.h"
#include "Player.h"
#include "Inventory.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Init()
{
	//
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->BeginPlay();

	for (UI* ui : _uis)
		ui->BeginPlay();
}

void Scene::Update()
{
	if (_isPaused)
		return;

	for (const vector<Actor*> actors : _actors)
		for (Actor* actor : actors)
			actor->Tick();

	for (UI* ui : _uis)
	{
		if (ui == nullptr)
			return;

		if (ui->GetVisible())
			ui->Tick();
	}

	Chat* chat = FindUI<Chat>(_uis);
	if(chat)
		chat->Tick();

	Inventory* inven = FindUI<Inventory>(_uis);
	if (inven)
		inven->Tick();
}

void Scene::Render(HDC hdc)
{
	// y 좌표가 더 큰쪽이 위로 오도록 설정
	vector<Actor*>& actors = _actors[LAYER_OBJECT];
	std::sort(actors.begin(), actors.end(), [=](Actor* a, Actor* b)
		{
			return a->GetPos().y < b->GetPos().y;
		});

	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->Render(hdc);

	for (UI* ui : _uis)
		if (ui->GetVisible())
			ui->Render(hdc);
}

void Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	_actors[actor->GetLayer()].push_back(actor);
}

void Scene::RemoveActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	vector<Actor*>& v = _actors[actor->GetLayer()];

	v.erase(std::remove(v.begin(), v.end(), actor), v.end());
}

void Scene::AddUI(UI* ui)
{
	if (ui == nullptr)
		return;

	_uis.push_back(ui);
}

void Scene::RemoveUI(UI* ui)
{
	if (ui == nullptr)
		return;

	vector<UI*>& v = _uis;

	v.erase(std::remove(v.begin(), v.end(), ui), v.end());
}

Creature* Scene::GetCreatureAt(Vec2Int cellPos)
{
	for (auto* actor : _actors[LAYER_OBJECT])
	{
		Creature* creature = dynamic_cast<Creature*>(actor);
		if (creature && creature->GetCellPos() == cellPos)
			return creature;
	}

	return nullptr;
}

Player* Scene::GetPlayerByID(uint64 objectId)
{
	for (auto& actor : _actors)
	{
		for (auto& creature : actor)
		{
			Player* player = dynamic_cast<Player*>(creature);

			if (!player)
				continue;

			if (player->info.objectid() == objectId)
				return player ? player : 0;
		}
	}
}
