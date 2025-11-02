#include "pch.h"
#include "HitEffect.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

HitEffect::HitEffect()
{
	SetLayer(LAYER_EFFECT);
	UpdateAnimation();
}

HitEffect::~HitEffect()
{

}

void HitEffect::BeginPlay()
{
	Super::BeginPlay();
}

void HitEffect::Tick()
{
	Super::Tick();

	if (IsAnimationEnded())
	{
		Scene* scene = GET(SceneMgr)->GetCurrentScene();
		scene->RemoveActor(this);
	}
}

void HitEffect::Render(HDC hdc)
{
	Super::Render(hdc);

}

void HitEffect::UpdateAnimation()
{
	SetFlipbook(GET(ResMgr)->GetFlipbook(L"FB_Hit"));
}
