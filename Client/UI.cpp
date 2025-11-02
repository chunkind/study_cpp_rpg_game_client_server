#include "pch.h"
#include "UI.h"
#include "InputMgr.h"

UI::UI()
{

}

UI::~UI()
{

}

void UI::BeginPlay()
{

}

void UI::Tick()
{

}

void UI::Render(HDC hdc)
{
	Utils::DrawRect(hdc, _pos, _size.x, _size.y);
}

RECT UI::GetRect()
{
	RECT rect =
	{
		_pos.x - _size.x / 2,
		_pos.y - _size.y / 2,
		_pos.x + _size.x / 2,
		_pos.y + _size.y / 2
	};

	return rect;
}

bool UI::IsMouseInRect()
{
	RECT rect = GetRect();

	POINT mousePos = GET(InputMgr)->GetMousePos();

	//return ::PtInRect(&rect, mousePos);
	if (mousePos.x < rect.left)
		return false;
	if (mousePos.x > rect.right)
		return false;
	if (mousePos.y < rect.top)
		return false;
	if (mousePos.y > rect.bottom)
		return false;

	return true;
}
