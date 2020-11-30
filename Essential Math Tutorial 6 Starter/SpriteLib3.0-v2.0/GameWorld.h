#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
class GameWorld : public Scene
{
public:
	GameWorld(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	//void KeyboardUp override;

	float treeX = -180.f;
	float treeY = -50.f;

protected:
	PhysicsPlaygroundListener listener;
};

