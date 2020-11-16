#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class Cafe : public Scene
{
public:
	Cafe(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	bool done = false;

protected:
	PhysicsPlaygroundListener listener;
	int test;
	int check = 0;
};
