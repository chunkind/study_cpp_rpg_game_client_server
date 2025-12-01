#pragma once

#include "FlipbookActor.h"
#include "Creature.h"

class Sprite;

class Tree : public Creature
{
	using Super = Creature;
public:
	Tree();
	virtual ~Tree() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	virtual void TickIdle() override;
	virtual void TickMove() override;
	virtual void TickDead() override;

	virtual void UpdateAnimation() override;

protected:
	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _filebookDead[4] = {};

	Sprite* _sprite = nullptr;
	bool _isCenterPos = true;
};