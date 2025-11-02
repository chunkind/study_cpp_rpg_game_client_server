#include "pch.h"
#include "RectBar.h"

void RectBar::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();
	
	::TransparentBlt(hdc,
		(int32)_pos.x - size.x / 2,
		(int32)_pos.y - size.y / 2,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		RGB(255, 0, 255));
	
	Utils::DrawRectColored(hdc, Vec2(static_cast<float>(_pos.x + 8), static_cast<float>(_pos.y)), 100, 13, _color);
}
