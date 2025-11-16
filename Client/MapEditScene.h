#pragma once
#include "Scene.h"

class SpriteActor;

class MapEditScene : public Scene
{
	using Super = Scene;

public:
	MapEditScene();
	virtual ~MapEditScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void LoadMap();

	Vec2 GetPos() { return _pos; }

private:
	Vec2 _pos = { 0, 0 };
	SpriteActor* background = nullptr;
};

