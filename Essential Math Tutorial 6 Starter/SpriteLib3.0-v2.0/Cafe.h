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

	bool done = false;

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
	//arrat to hold blend name images
	std::string blendNames[3] = { "bluemountain.png", "kilimanjaro.png", "mocha.png" };
	std::string milkNames[4] = { "none.png", "alittle.png", "regular.png", "lots.png" };
	std::string sugarNames[4] = { "none.png", "onespoon.png", "twospoons.png", "threespoons.png" };
	std::string cupNames[3] = { "small.png", "medium.png", "large.png" };

	double timePassed = 0;

	bool gameOver = false;
	int timesUpSign;

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
};
