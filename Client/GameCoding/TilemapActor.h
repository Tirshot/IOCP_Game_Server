#pragma once
#include "Actor.h"

class Tilemap;

class TilemapActor : public Actor
{
	using Super = Actor;

public:
	TilemapActor();
	virtual ~TilemapActor();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void TilePicking();

	void SetTilemap(shared_ptr<Tilemap> tilemap) { _tilemap = tilemap; }
	shared_ptr<Tilemap> GetTilemap() { return _tilemap; }

	void SetShowDebug(bool showDebug) { _showDebug = showDebug; }

protected:
	shared_ptr<Tilemap> _tilemap = nullptr;
	// �ΰ��ӿ��� O, X�� ǥ��
	bool _showDebug = false;
};

