#pragma once

#include "Trigger.h"
#include "ECS.h"

class TranslateTrigger : public Trigger
{
public: 
	TranslateTrigger();

	void OnEnter() override;
	void OnExit() override;
protected:

	bool triggered = false;
};

