#include "CollisionFlag.h"


void CollisionFlag::SetCollisionTracker(bool check)
{
	m_collisionCheck = check;
}

bool CollisionFlag::GetCollisionTracker()
{
	return m_collisionCheck;
}
