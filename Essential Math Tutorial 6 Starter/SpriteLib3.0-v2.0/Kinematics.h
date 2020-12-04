#pragma once

class Kinematics
{
public:
	void SetPosition();
	void SetParent(int parent);
	void SetChild(int child);
	void SetOffset(float x, float y);

private:
	int kinParent = 0;
	int kinChild = 0;
	float offsetX = 0.f;
	float offsetY = 0.f;
};

