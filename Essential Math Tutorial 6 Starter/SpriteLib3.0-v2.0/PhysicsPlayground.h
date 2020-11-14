#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int puzzleWall1 = 0;
	int puzzleWall2 = 0;
	int puzzleStart2 = 0;
	int bridge = 0;
	bool triggered2 = false;
	float ball = 0;
	PhysicsBody puzzleStart2Body;
	float puzzleStart2X = 0.f;
	float puzzleStart2Y = 0.f;

};
