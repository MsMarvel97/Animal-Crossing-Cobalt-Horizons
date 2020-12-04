#pragma once
class CollisionFlag
{
public:
	void SetCollisionTracker(bool check);
	bool GetCollisionTracker();

protected:
	bool m_collisionCheck = false;
};

