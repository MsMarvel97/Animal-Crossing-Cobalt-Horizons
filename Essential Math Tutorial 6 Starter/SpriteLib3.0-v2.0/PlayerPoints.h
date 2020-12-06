#pragma once
class PlayerPoints
{
public:
	void GainPeachPoint();
	int GetPeachPoints();

	void SetStun(bool stunned);
	bool GetStun();

	void SetPointGain(bool gain);
	bool GetPointGain();

protected:
	int peachPoints = 0;
	bool gainPoint = false;
	bool stun = false;
};

