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
	// ��� �������� ..
	//GET(SceneMgr)->Clear();
	//GET(ResMgr)->Clear();

	_CrtDumpMemoryLeaks();
}

void Core::Init(HWND hwnd)
{
	_hwnd = hwnd;
	hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	hdcBack = ::CreateCompatibleDC(hdc); // hdc�� ȣȯ�Ǵ� DC�� ����
	_bmpBack = ::CreateCompatibleBitmap(hdc, _rect.right, _rect.bottom); // hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
	HBITMAP prev = (HBITMAP)::SelectObject(hdcBack, _bmpBack); // DC�� BMP�� ����
	::DeleteObject(prev);

	GET(TimeMgr)->Init();
	GET(InputMgr)->Init(hwnd);
	GET(SceneMgr)->Init();
	GET(ResMgr)->Init(hwnd, fs::path(L"C:\\git\\study_cpp_rpg_game_client_server\\Resources"));

	GET(SoundMgr)->Init(hwnd);

	GET(SceneMgr)->ChangeScene(SceneType::GameScene);

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
	RECT clientRect;
	::GetClientRect(_hwnd, &clientRect);
	::SetStretchBltMode(hdc, HALFTONE);
	::StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
		hdcBack, 0, 0, _rect.right, _rect.bottom, SRCCOPY);
	::PatBlt(hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
