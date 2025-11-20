#include "pch.h"
#include "GameScene.h"
#include "Utils.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Sprite.h"
#include "Actor.h"
#include "SpriteActor.h"
#include "Player.h"
#include "Flipbook.h"
#include "UI.h"
#include "Button.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "SoundMgr.h"
#include "Sound.h"
#include "Monster.h"
#include "MyPlayer.h"
#include "SceneMgr.h"
#include "RectBar.h"
#include "Core.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	GET(ResMgr)->LoadTexture(L"Stage01", L"Sprite\\Map\\main01.bmp");
	GET(ResMgr)->LoadTexture(L"Tile", L"Sprite\\Map\\tile.bmp", RGB(255, 0, 255));
	GET(ResMgr)->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
	GET(ResMgr)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));
	GET(ResMgr)->LoadTexture(L"Potion", L"Sprite\\UI\\Mp.bmp");

	GET(ResMgr)->LoadTexture(L"Player", L"Sprite\\Player\\player.bmp", RGB(255, 0, 255));

	GET(ResMgr)->LoadTexture(L"Dack", L"Sprite\\Monster\\dack.bmp", RGB(255, 0, 255));
	GET(ResMgr)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0, 0, 0));

	GET(ResMgr)->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	GET(ResMgr)->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	GET(ResMgr)->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp");

	GET(ResMgr)->LoadTexture(L"RectBar", L"Sprite\\UI\\hp_mp_bar.bmp");

	GET(ResMgr)->CreateSprite(L"Stage01", GET(ResMgr)->GetTexture(L"Stage01"));
	GET(ResMgr)->CreateSprite(L"TileO", GET(ResMgr)->GetTexture(L"Tile"), 0, 0, 32, 32);
	GET(ResMgr)->CreateSprite(L"TileX", GET(ResMgr)->GetTexture(L"Tile"), 32, 0, 32, 32);
	GET(ResMgr)->CreateSprite(L"Start_Off", GET(ResMgr)->GetTexture(L"Start"), 0, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"Start_On", GET(ResMgr)->GetTexture(L"Start"), 150, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"Edit_Off", GET(ResMgr)->GetTexture(L"Edit"), 0, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"Edit_On", GET(ResMgr)->GetTexture(L"Edit"), 150, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"Exit_Off", GET(ResMgr)->GetTexture(L"Exit"), 0, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"Exit_On", GET(ResMgr)->GetTexture(L"Exit"), 150, 0, 150, 150);
	GET(ResMgr)->CreateSprite(L"HpBar", GET(ResMgr)->GetTexture(L"RectBar"), 0, 0, 32 * 4, 32);
	GET(ResMgr)->CreateSprite(L"MpBar", GET(ResMgr)->GetTexture(L"RectBar"), 0, 32, 32 * 4, 32);

	LoadMap();
	LoadPlayer();
	LoadMonster();
	LoadProjectiles();
	LoadEffect();
	LoadTilemap();
	LoadUI();

	GET(ResMgr)->LoadSound(L"BGM", L"Sound\\BGM.wav");
	GET(ResMgr)->LoadSound(L"Attack", L"Sound\\Sword.wav");

	//SpawnObjectAtRandomPos<MyPlayer>();
	//SpawnObjectAtRandomPos<Monster>();
	//SpawnObject<Monster>(Vec2Int{7, 7});

	Super::Init();
}

void GameScene::Update()
{
	Super::Update();

	float deltaTime = DT;

	TickMonsterSpawn();
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

void GameScene::AddActor(Actor* actor)
{
	Super::AddActor(actor);

	Monster* creature = dynamic_cast<Monster*>(actor);
	if (creature)
	{
		_monsterCount++;
	}
}

void GameScene::RemoveActor(Actor* actor)
{
	Super::RemoveActor(actor);

	Monster* creature = dynamic_cast<Monster*>(actor);
	if (creature)
	{
		_monsterCount--;
	}
}

void GameScene::LoadMap()
{
	Sprite* sprite = GET(ResMgr)->GetSprite(L"Stage01");

	SpriteActor* background = new SpriteActor();
	background->SetSprite(sprite);
	background->SetLayer(LAYER_BACKGROUND);
	const Vec2Int size = sprite->GetSize();
	background->SetPos(Vec2(size.x / 2, size.y / 2));

	AddActor(background);
}

void GameScene::LoadPlayer()
{
	// IDLE
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_IdleUp");
		fb->SetInfo({ texture, L"FB_MoveUp", {32*2, 32*2}, 0, 4, 0, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_IdleDown");
		fb->SetInfo({ texture, L"FB_MoveDown", {32*2, 32*2}, 0, 4, 1, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_IdleLeft");
		fb->SetInfo({ texture, L"FB_MoveLeft", {32*2, 32*2}, 0, 4, 2, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_IdleRight");
		fb->SetInfo({ texture, L"FB_MoveRight", {32*2, 32*2}, 0, 4, 3, 0.5f });
	}
	// MOVE
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_MoveUp");
		fb->SetInfo({ texture, L"FB_MoveUp", {32*2, 32 * 2}, 0, 4, 4, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_MoveDown");
		fb->SetInfo({ texture, L"FB_MoveDown", {32 * 2, 32 * 2}, 0, 4, 5, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_MoveLeft");
		fb->SetInfo({ texture, L"FB_MoveLeft", {32 * 2, 32 * 2}, 0, 4, 6, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_MoveRight");
		fb->SetInfo({ texture, L"FB_MoveRight", {32 * 2, 32 * 2}, 0, 4, 7, 0.5f });
	}
	// SKILL
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_AttackUp");
		fb->SetInfo({ texture, L"FB_MoveUp", {32*2, 32 * 2}, 0, 4, 8, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_AttackDown");
		fb->SetInfo({ texture, L"FB_MoveDown", {32 * 2, 32 * 2}, 0, 4, 9, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_AttackLeft");
		fb->SetInfo({ texture, L"FB_MoveLeft", {32 * 2, 32 * 2}, 0, 4, 10, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_AttackRight");
		fb->SetInfo({ texture, L"FB_MoveRight", {32 * 2, 32 * 2}, 0, 4, 11, 0.5f, false });
	}
	// BOW
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_BowUp");
		fb->SetInfo({ texture, L"FB_BowUp", {32 * 2, 32 * 2}, 0, 4, 12, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_BowDown");
		fb->SetInfo({ texture, L"FB_BowDown", {32 * 2, 32 * 2}, 0, 4, 13, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_BowLeft");
		fb->SetInfo({ texture, L"FB_BowLeft", {32 * 2, 32 * 2}, 0, 4, 14, 0.5f, false });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Player");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_BowRight");
		fb->SetInfo({ texture, L"FB_BowRight", {32 * 2, 32 * 2}, 0, 4, 15, 0.5f, false });
	}

}

void GameScene::LoadMonster()
{
	// MOVE
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Dack");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_SnakeUp");
		fb->SetInfo({ texture, L"FB_SnakeUp", {32*2, 32 * 2}, 0, 3, 0, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Dack");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_SnakeDown");
		fb->SetInfo({ texture, L"FB_SnakeDown", {32 * 2, 32 * 2}, 0, 3, 1, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Dack");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_SnakeLeft");
		fb->SetInfo({ texture, L"FB_SnakeLeft", {32 * 2, 32 * 2}, 0, 3, 2, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Dack");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_SnakeRight");
		fb->SetInfo({ texture, L"FB_SnakeRight", {32 * 2, 32 * 2}, 0, 3, 3, 0.5f });
	}
}

void GameScene::LoadProjectiles()
{
	// MOVE
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Arrow");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_ArrowUp");
		fb->SetInfo({ texture, L"FB_ArrowUp", {100, 100}, 0, 0, 3, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Arrow");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_ArrowDown");
		fb->SetInfo({ texture, L"FB_ArrowDown", {100, 100}, 0, 0, 0, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Arrow");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_ArrowLeft");
		fb->SetInfo({ texture, L"FB_ArrowLeft", {100, 100}, 0, 0, 1, 0.5f });
	}
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Arrow");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_ArrowRight");
		fb->SetInfo({ texture, L"FB_ArrowRight", {100, 100}, 0, 0, 2, 0.5f });
	}
}

void GameScene::LoadEffect()
{
	{
		Texture* texture = GET(ResMgr)->GetTexture(L"Hit");
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(L"FB_Hit");
		fb->SetInfo({ texture, L"FB_Hit", {50, 47}, 0, 5, 0, 0.5f, false });
	}
}

void GameScene::LoadTilemap()
{
	TilemapActor* actor = new TilemapActor();
	AddActor(actor);

	_tilemapActor = actor;
	{
		auto* tm = GET(ResMgr)->CreateTilemap(L"Tilemap_01");
		tm->SetMapSize({100, 100});
		tm->SetTileSize(TILE_SIZE::TILE_SIZEX);

		GET(ResMgr)->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap_01.txt");

		_tilemapActor->SetTilemap(tm);
		_tilemapActor->SetShowDebug(true);
	}
}

void GameScene::LoadUI()
{
	int screenWidth = GET(Core)->GetScreenWidth();
	int screenHeight = GET(Core)->GetScreenHeight();

	Sprite* hpSprite = GET(ResMgr)->GetSprite(L"HpBar");
	Sprite* mpSprite = GET(ResMgr)->GetSprite(L"MpBar");

	const Vec2Int hpSize = hpSprite->GetSize();
	const Vec2Int mpSize = mpSprite->GetSize();

	RectBar* hpBar = new RectBar();
	hpBar->SetUIName("HpBar");
	hpBar->SetSprite(hpSprite);
	hpBar->SetColor(RGB(255, 0, 0));
	hpBar->SetPos(Vec2(screenWidth - hpSize.x/2 - 10, screenHeight - (hpSize.y/2*3)));

	RectBar* mpBar = new RectBar();
	mpBar->SetUIName("MpBar");
	mpBar->SetSprite(mpSprite);
	mpBar->SetColor(RGB(0, 0, 255));
	mpBar->SetPos(Vec2(screenWidth - mpSize.x/2 - 10, screenHeight - (mpSize.y/2)));

	AddUI(hpBar);
	AddUI(mpBar);
}

void GameScene::Handle_S_AddObject(Protocol::S_AddObject& pkt)
{
	uint64 myPlayerId = GET(SceneMgr)->GetMyPlayerId();

	const int32 size = pkt.objects_size();
	for (int32 i = 0; i < size; i++)
	{
		const Protocol::ObjectInfo& info = pkt.objects(i);
		if (myPlayerId == info.objectid())
			continue;

		if (info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
		{
			Player* player = SpawnObject<Player>(Vec2Int{ info.posx(), info.posy() });
			player->SetDir(info.dir());
			player->SetState(info.state());
			player->info = info;
		}
		else if (info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
		{
			Monster* monster = SpawnObject<Monster>(Vec2Int{ info.posx(), info.posy() });
			monster->SetDir(info.dir());
			monster->SetState(info.state());
			monster->info = info;
		}
	}
}

void GameScene::Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt)
{
	const int32 size = pkt.ids_size();
	for (int32 i = 0; i < size; i++)
	{
		int32 id = pkt.ids(i);

		GameObject* object = GetObject(id);
		if (object)
			RemoveActor(object);
	}
}

void GameScene::Handle_S_Attack(Protocol::S_Attack& pkt)
{

}

GameObject* GameScene::GetObject(uint64 id)
{
	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(actor);
		if (gameObject && gameObject->info.objectid() == id)
			return gameObject;
	}

	return nullptr;
}

Player* GameScene::FindClosestPlayer(Vec2Int pos)
{
	float best = FLT_MAX;
	Player* ret = nullptr;

	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		Player* player = dynamic_cast<Player*>(actor);
		if (player)
		{
			Vec2Int dir = pos - player->GetCellPos();
			float dist = dir.LengthSquared();
			if (dist < best)
			{
				dist = best;
				ret = player;
			}
		}
	}

	return ret;
}

// A* -> Dijikstra -> BFS -> Graph
// PQ
bool GameScene::FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth)
{
	// F = G + H
	// F = 최종 점수(작을 수록 좋음)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용
	// H = 목적지에서 해당 좌표까지 이동하는데 드는 비용
	int32 depth = abs(src.y - dest.y) + abs(src.x - dest.x);
	if (depth >= maxDepth)
		return false;

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	map<Vec2Int, int32> best;
	map<Vec2Int, Vec2Int> parent;

	// 초기값
	{
		int32 cost = abs(dest.y - src.y) + abs(dest.x - src.x);

		pq.push(PQNode(cost, src));
		best[src] = cost;
		parent[src] = src;
	}

	Vec2Int front[4] =
	{
		{0, -1},
		{0, 1},
		{-1, 0},
		{1, 0},
	};

	bool found = false;

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		PQNode node = pq.top();
		pq.pop();

		// 더 짧은 경로를 뒤늦게 찾았다면 스킵
		if (best[node.pos] < node.cost)
			continue;

		// 목적지에 도착했으면 바로 종료
		if (node.pos == dest)
		{
			found = true;
			break;
		}

		// 방문
		for (int32 dir = 0; dir < 4; dir++)
		{
			Vec2Int nextPos = node.pos + front[dir];

			if (CanGo(nextPos) == false)
				continue;

			int32 depth = abs(src.y - nextPos.y) + abs(src.x - nextPos.x);
			if (depth >= maxDepth)
				continue;

			int32 cost = abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x);
			int32 bestValue = best[nextPos];
			if (bestValue != 0)
			{
				// 다른 경로에서 더 빠른 길을 찾았으면 스킵
				if (bestValue <= cost)
					continue;
			}

			// 예약 진행
			best[nextPos] = cost;
			pq.push(PQNode(cost, nextPos));
			parent[nextPos] = node.pos;
		}
	}

	if (found == false)
	{
		float bestScore = FLT_MAX;

		for (auto& item : best)
		{
			Vec2Int pos = item.first;
			int32 score = item.second;

			// 동점이라면, 최초 위치에서 가장 덜 이동하는 쪽으로
			if (bestScore == score)
			{
				int32 dist1 = abs(dest.x - src.x) + abs(dest.y - src.y);
				int32 dist2 = abs(pos.x - src.x) + abs(pos.y - src.y);
				if (dist1 > dist2)
					dest = pos;
			}
			else if (bestScore > score)
			{
				dest = pos;
				bestScore = score;
			}
		}
	}

	path.clear();
	Vec2Int pos = dest;

	while (true)
	{
		path.push_back(pos);

		// 시작점
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(path.begin(), path.end());
	return true;
}

bool GameScene::CanGo(Vec2Int cellPos)
{
	if (_tilemapActor == nullptr)
		return false;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return false;

	Tile* tile = tm->GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// 몬스터 충돌?
	if (GetCreatureAt(cellPos) != nullptr)
		return false;

	return tile->value != 1;
}

Vec2 GameScene::ConvertPos(Vec2Int cellPos)
{
	Vec2 ret = {};

	if (_tilemapActor == nullptr)
		return ret;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return ret;

	int32 size = tm->GetTileSize();
	Vec2 pos = _tilemapActor->GetPos();

	ret.x = pos.x + cellPos.x * size + (size / 2);
	ret.y = pos.y + cellPos.y * size + (size / 2);

	return ret;
}

Vec2Int GameScene::GetRandomEmptyCellPos()
{
	Vec2Int ret = { -1, -1 };

	if (_tilemapActor == nullptr)
		return ret;

	Tilemap* tm = _tilemapActor->GetTilemap();
	if (tm == nullptr)
		return ret;

	Vec2Int size = tm->GetMapSize();

	// 몇 번 시도?
	while (true)
	{
		int32 x = rand() % size.x;
		int32 y = rand() % size.y;
		Vec2Int cellPos{ x, y };

		if (CanGo(cellPos))
			return cellPos;
	}
}

GameObject* GameScene::GetGameObjectAt(Vec2Int cellPos)
{
	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(actor);
		if (gameObject && gameObject->info.posx() == cellPos.x && gameObject->info.posy() == cellPos.y)
			return gameObject;
	}

	return nullptr;
}

void GameScene::TickMonsterSpawn()
{
	return;

	if (_monsterCount < DESIRED_COUNT)
		SpawnObjectAtRandomPos<Monster>();
}

