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
	void UpdateSprites();
	std::string TimerOnes(std::string digit);
	std::string TimerTens(std::string digit);
	std::string TimerMinutes(std::string digit);

protected:
	PhysicsPlaygroundListener listener;
	int ball = 0;
	int floor = 0;
	float timer = 0.f;
	float limit = 2.f;
	bool countDown = true;
	bool start = false;
	
	int minutesCount = 1;
	int tentsCount = 3;
	int onesCount = 0;

	int frames = 0;
	int timerFrames = 0;

	int fruitCounter = 0;
	int stunTracker = 0;
	int player = 0;
	int stunIcon = 0;

	int timerMinutes = 0;
	int timerSecondsTens = 0;
	int timerSecondsOnes = 0;
};
