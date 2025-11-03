#include "pch.h"
#include "Arrow.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "SceneMgr.h"
#include "GameScene.h"
#include "Creature.h"
#include "HitEffect.h"

Arrow::Arrow()
{
	_flipbookMove[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_ArrowUp");
	_flipbookMove[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_ArrowDown");
	_flipbookMove[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_ArrowLeft");
	_flipbookMove[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_ArrowRight");

}

Arrow::~Arrow()
{

}

void Arrow::BeginPlay()
{
	Super::BeginPlay();
	UpdateAnimation();
}

void Arrow::Tick()
{
	Super::Tick();

}

void Arrow::Render(HDC hdc)
{
	Super::Render(hdc);


}

void Arrow::TickIdle()
{
	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];

	if (CanGo(nextPos))
	{
		SetCellPos(nextPos);
		SetState(MOVE);
	}
	else
	{
		Creature* creature = scene->GetCreatureAt(nextPos);
		if (creature)
		{
			scene->SpawnObject<HitEffect>(nextPos);
			//creature->OnDamaged(this);
		}

		scene->RemoveActor(this);
	}
}

void Arrow::TickMove()
{
	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 5.f)
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 600 * DT;
			break;
		case DIR_DOWN:
			_pos.y += 600 * DT;
			break;
		case DIR_LEFT:
			_pos.x -= 600 * DT;
			break;
		case DIR_RIGHT:
			_pos.x += 600 * DT;
			break;
		}
	}
}

void Arrow::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[info.dir()]);
}
