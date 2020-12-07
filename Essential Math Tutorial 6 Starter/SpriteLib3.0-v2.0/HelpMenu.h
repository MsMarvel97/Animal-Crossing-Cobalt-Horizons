#pragma once
#include "Scene.h"

class HelpMenu : public Scene
{
public:
	HelpMenu(std::string name);
	void InitScene(float windowWidth, float windowHeight) override;
	void Update() override;
	void KeyboardDown() override;
};

