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

/**
* clamp(값, 최소, 최대)
  - 값이 최소보다 작으면 → 최소 반환
  - 값이 최대보다 크면 → 최대 반환
  - 범위 안이면 → 그대로 반환
  예시:
  clamp(50, 100, 500)   → 100  (50이 최소값 100보다 작으니까)
  clamp(300, 100, 500)  → 300  (범위 안이니까 그대로)
  clamp(700, 100, 500)  → 500  (700이 최대값 500보다 크니까)
*/
void CameraComponent::TickComponent()
{
    if (_owner == nullptr)
        return;

    // 카메라가 따라갈 대상(플레이어)의 월드 좌표
    Vec2 pos = _owner->GetPos();

    // 맵 전체 픽셀 크기 (타일 수 x 타일 픽셀 크기)
    float mapFullWidth = GMapWidth * GPixcelWidth;
    float mapFullHeight = GMapHeight * GPixcelHeight;

    // 카메라 클램프: 화면 밖으로 맵이 벗어나지 않도록 제한
    // 최소값 = 화면 절반 (좌상단이 맵 원점 밖으로 안 나감)
    // 최대값 = 맵 크기 - 화면 절반 (우하단이 맵 끝 밖으로 안 나감)
    pos.x = ::clamp(pos.x, (float)(GWinSizeX / 2), mapFullWidth - GWinSizeX / 2);
    pos.y = ::clamp(pos.y, (float)(GWinSizeY / 2), mapFullHeight - GWinSizeY / 2);

    GET(SceneMgr)->SetCameraPos(pos);
}

void CameraComponent::Render(HDC hdc)
{

}
