#include "pch.h"
#include "CameraComponent.h"
#include "Actor.h"
#include "SceneMgr.h"

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::BeginPlay()
{

}

void CameraComponent::TickComponent()
{
	if (_owner == nullptr)
		return;

	Vec2 pos = _owner->GetPos();

	// TEMP
	pos.x = ::clamp(pos.x, 400.f, 3200.f - 400.f);
	pos.y = ::clamp(pos.y, 300.f, 3280.f - 300.f);

	GET(SceneMgr)->SetCameraPos(pos);
}

void CameraComponent::Render(HDC hdc)
{

}
