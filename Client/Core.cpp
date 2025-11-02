#include "pch.h"
#include "Core.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "SoundMgr.h"
#include "NetMgr.h"

Core::Core()
{

}

Core::~Core()
{
	// 사실 마지막에 ..
	//GET(SceneMgr)->Clear();
	//GET(ResMgr)->Clear();

	_CrtDumpMemoryLeaks();
}

void Core::Init(HWND hwnd)
{
	_hwnd = hwnd;
	hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	hdcBack = ::CreateCompatibleDC(hdc); // hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);

	GET(TimeMgr)->Init();
	GET(InputMgr)->Init(hwnd);
	GET(SceneMgr)->Init();
	GET(ResMgr)->Init(hwnd, fs::path(L"C:\\git\\study_cpp_rpg_game_client_server\\Resources"));

	GET(SoundMgr)->Init(hwnd);

	GET(SceneMgr)->ChangeScene(SceneType::DevScene);

	GET(NetMgr)->Init();
}

void Core::Update()
{
	GET(TimeMgr)->Update();
	GET(InputMgr)->Update();
	GET(SceneMgr)->Update();
	GET(NetMgr)->Update();
}

void Core::Render()
{
	GET(SceneMgr)->Render(hdcBack);

	uint32 fps = GET(TimeMgr)->GetFps();

	{
		POINT mousePos = GET(InputMgr)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, DT);
		::TextOut(hdcBack, 550, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// Double Buffering
	::BitBlt(hdc, 0, 0, _rect.right, _rect.bottom, hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
