#include "pch.h"
#include "GameScene.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Player.h"


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	// ���� 1���� ����
	{
		//for (int32 i = 0; i < 5; i++)
		{
			Monster* monster = GET_SINGLE(ObjectManager)->CreateObject<Monster>();
			
			GET_SINGLE(ObjectManager)->Add(monster);
		}
	}
}

void GameScene::Update()
{
	const vector<Object*> objects = GET_SINGLE(ObjectManager)->GetObjects();

	// ������ object�� ���� update() ����
	for (auto object : objects)
		object->Update();
}

void GameScene::Render(HDC hdc)
{
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

	// ������ object�� ���� Render() ����
	for (auto object : objects)
		object->Render(hdc);

}
