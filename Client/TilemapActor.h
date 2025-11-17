#pragma once
#include "Actor.h"

class Tilemap;

enum TILE_SIZE
{
	TILE_WIDTH = 100,
	TILE_HEIGHT = 100,
	TILE_SIZEX = 32,
	TILE_SIZEY = 32,
};

class TilemapActor : public Actor
{
	using Super = Actor;
public:
	TilemapActor();
	virtual ~TilemapActor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void TickPicking();

	void SetTilemap(Tilemap* tilemap) { _tilemap = tilemap; }
	Tilemap* GetTilemap() { return _tilemap; }

	void SetShowDebug(bool showDebug) { _showDebug = showDebug; }

protected:
	Tilemap* _tilemap = nullptr;
	bool _showDebug = false;
};

