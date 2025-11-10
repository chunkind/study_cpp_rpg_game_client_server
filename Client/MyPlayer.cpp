#include "pch.h"
#include "MyPlayer.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "SceneMgr.h"
#include "GameScene.h"
#include "Arrow.h"
#include "HitEffect.h"

MyPlayer::MyPlayer()
{
	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);
}

MyPlayer::~MyPlayer()
{

}

void MyPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void MyPlayer::Tick()
{
	Super::Tick();

	SyncToServer();
}

void MyPlayer::Render(HDC hdc)
{
	Super::Render(hdc);
}

void MyPlayer::TickInput()
{
	_keyPressed = true;

	if (GET(InputMgr)->GetButton(KeyType::W))
	{
		SetDir(DIR_UP);
	}
	else  if (GET(InputMgr)->GetButton(KeyType::S))
	{
		SetDir(DIR_DOWN);
	}
	else if (GET(InputMgr)->GetButton(KeyType::A))
	{
		SetDir(DIR_LEFT);
	}
	else if (GET(InputMgr)->GetButton(KeyType::D))
	{
		SetDir(DIR_RIGHT);
	}
	else
	{
		_keyPressed = false;
	}

	if (GET(InputMgr)->GetButtonDown(KeyType::KEY_1))
	{
		SetWeaponType(WeaponType::Sword);
	}
	else if (GET(InputMgr)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(WeaponType::Bow);
	}
	else if (GET(InputMgr)->GetButtonDown(KeyType::KEY_3))
	{
		SetWeaponType(WeaponType::Staff);
	}

	if (GET(InputMgr)->GetButton(KeyType::SpaceBar))
	{
		SetState(SKILL);
	}
}

void MyPlayer::TryMove()
{
	if (_keyPressed == false)
		return;

	Vec2Int deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	if (info.dir() == DIR_UP)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else  if (info.dir() == DIR_DOWN)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_LEFT)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_RIGHT)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
}

void MyPlayer::TickIdle()
{
	TickInput();
	TryMove();
}

void MyPlayer::TickMove()
{
	Super::TickMove();
}

void MyPlayer::TickSkill()
{
	Super::TickSkill();
}

void MyPlayer::SyncToServer()
{

	if (info.state() == Protocol::OBJECT_STATE_TYPE_DEAD || info.hp() <= 0) {
		SendBufferRef send = ClientPacketHandler::Make_C_RemoveObject();
		GET(NetMgr)->SendPacket(send);
	}

	if (_dirtyFlag == false)
		return;

	SendBufferRef sendBuffer = ClientPacketHandler::Make_C_Move();
	GET(NetMgr)->SendPacket(sendBuffer);

	if (info.state() == SKILL)
	{
		SendBufferRef send = ClientPacketHandler::Make_C_Attack();
		GET(NetMgr)->SendPacket(send);
	}

	
}
