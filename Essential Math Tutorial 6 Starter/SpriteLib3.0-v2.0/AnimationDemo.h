#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
class AnimationDemo : public Scene
{
public:
	AnimationDemo(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;


};
