#include "pch.h"
#include "MapEditScene.h"
#include "ResMgr.h"
#include "SpriteActor.h"
#include "Sprite.h"
#include "InputMgr.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "Component.h"
#include "CameraComponent.h"

MapEditScene::MapEditScene()
{
}

MapEditScene::~MapEditScene()
{
}

void MapEditScene::Init()
{
	GET(ResMgr)->LoadTexture(L"Stage01", L"Sprite\\Map\\map1.bmp");
	GET(ResMgr)->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(255, 0, 255));

	GET(ResMgr)->CreateSprite(L"Stage01", GET(ResMgr)->GetTexture(L"Stage01"));
	GET(ResMgr)->CreateSprite(L"TileO", GET(ResMgr)->GetTexture(L"Tile"), 0, 0, 32, 32);
	GET(ResMgr)->CreateSprite(L"TileX", GET(ResMgr)->GetTexture(L"Tile"), 32, 0, 32, 32);

	LoadMap();

	Tilemap* tm = new Tilemap();
	tm->SetMapSize(Vec2Int{ TILE_SIZE::TILE_WIDTH , TILE_SIZE::TILE_HEIGHT });
	tm->SetTileSize(TILE_SIZE::TILE_SIZEX);

	TilemapActor* ta = new TilemapActor();
	ta->SetShowDebug(true);
	ta->SetPos(Vec2{ 0, 0 });
	ta->SetLayer(LAYER_OBJECT);
	ta->SetTilemap(tm);

	AddActor(ta);

	Super::Init();
}

void MapEditScene::Update()
{
	if (GET(InputMgr)->GetButton(KeyType::W))
	{
		_pos.y += 10;

		for (int i = 0; i < _actors[LAYER_OBJECT].size(); i++)
		{
			Vec2 pos = _actors[LAYER_OBJECT][i]->GetPos();
			_actors[LAYER_OBJECT][i]->SetPos(Vec2{ pos.x , pos.y + 10 });
		}
	}
	else  if (GET(InputMgr)->GetButton(KeyType::S))
	{
		_pos.y -= 10;

		for (int i = 0; i < _actors[LAYER_OBJECT].size(); i++)
		{
			Vec2 pos = _actors[LAYER_OBJECT][i]->GetPos();
			_actors[LAYER_OBJECT][i]->SetPos(Vec2{ pos.x , pos.y - 10 });
		}
	}
	else if (GET(InputMgr)->GetButton(KeyType::A))
	{
		_pos.x += 10;

		for (int i = 0; i < _actors[LAYER_OBJECT].size(); i++)
		{
			Vec2 pos = _actors[LAYER_OBJECT][i]->GetPos();
			_actors[LAYER_OBJECT][i]->SetPos(Vec2{ pos.x + 10, pos.y});
		}
	}
	else if (GET(InputMgr)->GetButton(KeyType::D))
	{
		_pos.x -= 10;

		for (int i = 0; i < _actors[LAYER_OBJECT].size(); i++)
		{
			Vec2 pos = _actors[LAYER_OBJECT][i]->GetPos();
			_actors[LAYER_OBJECT][i]->SetPos(Vec2{ pos.x - 10, pos.y });
		}
	}

	background->SetPos(_pos);

	Super::Update();
}

void MapEditScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

void MapEditScene::LoadMap()
{
	Sprite* sprite = GET(ResMgr)->GetSprite(L"Stage01");

	background = new SpriteActor();
	background->SetCenterPos(false);
	background->SetSprite(sprite);
	background->SetLayer(LAYER_BACKGROUND);
	const Vec2Int size = sprite->GetSize();
	background->SetPos(Vec2(size.x/2, size.y/2));

	AddActor(background);
}