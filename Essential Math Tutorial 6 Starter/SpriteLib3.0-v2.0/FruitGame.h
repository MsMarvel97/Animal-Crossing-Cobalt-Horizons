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
	void UpdateFruitKinematics();

protected:
	PhysicsPlaygroundListener listener;
	int ball = 0;
	int floor = 0;
	float timer = 0.f;
	float limit = 2.f;
	bool countDown = true;
	bool start = false;
	int frames = 0;
	int fruitCounter = 0;
	int stunTracker = 0;
	int player = 0;
	int stunIcon = 0;
};
