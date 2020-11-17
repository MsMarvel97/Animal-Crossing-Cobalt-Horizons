#pragma once

class Checker
{
public:
	void SetCollisionTracker(bool check);
	bool GetCollisionTracker();

protected:
	bool m_collisionCheck = false;
};

