#include "pch.h"
#include "Creature.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "SceneMgr.h"
#include "GameScene.h"

Creature::Creature()
{

}

Creature::~Creature()
{

}

void Creature::BeginPlay()
{
	Super::BeginPlay();
}

void Creature::Tick()
{
	Super::Tick();

}

void Creature::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Creature::OnDamaged(Creature* attacker)
{
	if (attacker == nullptr)
		return;

	Stat& attackerStat = attacker->GetStat();
	Stat& stat = GetStat();

	int32 damage = attackerStat.attack - stat.defence;
	if (damage <= 0)
		return;

	int32 hp = info.hp();
	int32 afterHp = max(0, hp - damage);
	info.set_hp(afterHp);
	if (afterHp == 0)
	{
		_dirtyFlag = true;
		info.set_state(Protocol::OBJECT_STATE_TYPE_DEAD);
		Scene* scene = GET(SceneMgr)->GetCurrentScene();
		if (scene)
		{
			scene->RemoveActor(this);
		}
	}
}
