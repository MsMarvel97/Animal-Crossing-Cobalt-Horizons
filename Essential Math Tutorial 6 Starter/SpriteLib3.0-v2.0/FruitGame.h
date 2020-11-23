#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
class FruitGame : public Scene
{
public:
	FruitGame(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void NewFruits();

protected:
	PhysicsPlaygroundListener listener;
	int ball = 0;
	int floor;
	float timer=0.f;
	float limit = 2.f;
	bool countDown = true;
	bool start = false;
	int frames = 0;

};
