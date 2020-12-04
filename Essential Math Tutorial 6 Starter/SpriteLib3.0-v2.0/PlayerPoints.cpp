#include "PlayerPoints.h"

void PlayerPoints::GainPeachPoint()
{
	peachPoints++;
}

int PlayerPoints::GetPeachPoints()
{
	return peachPoints;
}

void PlayerPoints::SetStun(bool stunned)
{
	stun = stunned;
}

bool PlayerPoints::GetStun()
{
	return stun;
}
