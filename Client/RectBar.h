#pragma once

#include "UI.h"
#include "Sprite.h"

class RectBar : public UI
{
public:
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void SetColor(COLORREF color) { _color = color; }

protected:
	Sprite* _sprite = nullptr;
	COLORREF _color = RGB(0, 0, 0);
};

