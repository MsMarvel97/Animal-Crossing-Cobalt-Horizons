#pragma once
class PlayerPoints
{
public:
	void GainPeachPoint();
	int GetPeachPoints();

	void SetStun(bool stunned);
	bool GetStun();

protected:
	int peachPoints = 0;
	bool stun = false;
};

