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

	int booksFound = 0;
	bool bookOneTriggered = false;
	bool bookTwoTriggered = false;
	bool bookThreeTriggered = false;

	int rollsFound = 0;
	bool rollOneTriggered = false;
	bool rollTwoTriggered = false;
	bool rollThreeTriggered = false;

	int instrumentsFound = 0;
	bool instrumentOneTriggered = false;
	bool instrumentTwoTriggered = false;
	bool instrumentThreeTriggered = false;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_AudioDeviceID deviceID;

protected:
	PhysicsPlaygroundListener listener;
	int player;
	int bookOne;
	int bookTwo;
	int bookThree;
	int rollOne;
	int rollTwo;
	int rollThree;
	int instrumentOne;
	int instrumentTwo;
	int instrumentThree;

	int mStart = 0;
	int mEnd = 0;
	int kyStart = 0;
	int kyEnd = 0;
	int kaStart = 0;
	int kaEnd = 0;
	int wStart = 0;
	int wEnd = 0;
	int sStart = 0;
	int sEnd = 0;
};

