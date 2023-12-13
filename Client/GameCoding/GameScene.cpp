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
	// 몬스터 1마리 생성
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

	// 각각의 object에 대해 update() 수행
	for (auto object : objects)
		object->Update();
}

void GameScene::Render(HDC hdc)
{
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

	// 각각의 object에 대해 Render() 수행
	for (auto object : objects)
		object->Render(hdc);

}
