#pragma once

class Collider;

class CollisionMgr
{
	SING(CollisionMgr)

public:
	void Init();
	void Update();

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

private:
	vector<Collider*> _colliders;
};

