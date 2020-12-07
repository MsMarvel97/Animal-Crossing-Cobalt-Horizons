#pragma once
class MinigameQuestFlags
{
public:
	void SetChaos(bool flag);
	bool GetChaos();

	void SetFruit(bool flag);
	bool GetFruit();

private:
	bool cafeChaos = false;
	bool fruitShakedown = false;
};

