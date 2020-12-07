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
	//void KeyboardUp() override;
	void CurrentTime();
	bool done = false;
	void UpdateSprites();
	std::string TimerStrings(std::string digit);

	void UpdatePoints();
	//booleans for cups
	bool sC = true;
	bool mC = false;
	bool lC = false;

	//booleans for milk
	bool milk1 = true; //none
	bool milk2 = false; //a little bit
	bool milk3 = false; //regular
	bool milk4 = false; //lots

	//booleans for blends
	bool bBlend = true; //blue mountain
	bool kBlend = false; //kilimanjaro
	bool mBlend = false; //mocha

	//booleans for sugar
	bool b_noneSpoon = true;
	bool b_oneSpoon = false;
	bool b_twoSpoon = false;
	bool b_threeSpoon = false;

	//array to hold order using integers
	int custOrder[4] = { 1,1,1,1 };
	//array to hold current beverage selections using integers
	int beverage[4] = { 1,1,1,1 }; 
	//array to hold blend name images
	std::string blendNames[3] = { "bluemountain.png", "kilimanjaro.png", "mocha.png" };
	std::string milkNames[4] = { "none.png", "alittle.png", "regular.png", "lots.png" };
	std::string sugarNames[4] = { "none.png", "onespoon.png", "twospoons.png", "threespoons.png" };
	std::string cupNames[3] = { "small.png", "medium.png", "large.png" };

	//array to hold file name of numbers
	//std::string points [20] = {'digit'}
	std::string digitName = "Digit";
	std::string pngName = ".png";

	bool gameOver = false;
	int timesUpSign;

	int ordersComplete = 0;//tracks how many orders the user has successfully completed
	bool repeat = true;

	float timer = 0;
	float currentTime = 0;
	bool start = false;
	

protected:
	PhysicsPlaygroundListener listener;
	int toggleLayout;
	int player;
	//variables for cupstation
	int smallCup;
	int mediumCup;
	int largeCup;

	//varibles for milk station
	int m_none;
	int m_aLittleBit;
	int m_regular;
	int m_lots;

	//variable for blend station
	int blueMountainBlend;
	int kilimonjaroBlend;
	int mochaBlend;

	//variable for sugar station
	int noneSpoon;
	int oneSpoon;
	int twoSpoon;
	int threeSpoon;

	//variables for order log
	int blendOrder;
	int milkOrder;
	int sugarOrder;
	int cupOrder;

	//variables for timer
	int timerMinutes;
	int timerSecondsTens;
	int timerSecondsOnes;
	int timerFrames = 0;
	int minutesCount = 1;
	int tensCount = 3;
	int onesCount = 0;
	bool tens = false;
	bool minutes = false;

	//variables for points
	int pointTens;//sprite
	int pointOnes;//sprite
	int ten = 0;
	int one = 0;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_AudioDeviceID deviceID;  
};
