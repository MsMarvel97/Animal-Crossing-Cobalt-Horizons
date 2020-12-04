#pragma once

#include "Trigger.h"

class PeachTrigger : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;
protected:
	int playerPoints = 0;
	bool triggered = false;
};

