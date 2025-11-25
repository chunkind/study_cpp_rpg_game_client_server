#pragma once
#include "Projectile.h"

class Arrow : public Projectile
{
	using Super = Projectile;

public:
	Arrow();
	virtual ~Arrow() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void TickIdle() override;
	virtual void TickMove() override;

	virtual void UpdateAnimation() override;

public:
	void SetHit(bool hit) { _hit = hit; }

protected:
	Flipbook* _flipbookMove[4] = {};
	bool _hit = false;
};

