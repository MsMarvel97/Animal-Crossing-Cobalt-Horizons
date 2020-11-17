#include "Checker.h"

void Checker::SetCollisionTracker(bool check)
{
	m_collisionCheck = check;
}

bool Checker::GetCollisionTracker()
{
	return m_collisionCheck;
}
