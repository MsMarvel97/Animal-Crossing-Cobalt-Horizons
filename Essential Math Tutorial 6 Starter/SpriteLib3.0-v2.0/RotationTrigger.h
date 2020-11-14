#pragma once
#include "Trigger.h"

class RotationTrigger : public Trigger
{
public:
	RotationTrigger();

	void OnEnter() override;
	void OnExit() override;
protected:

	bool triggered = false;
};

