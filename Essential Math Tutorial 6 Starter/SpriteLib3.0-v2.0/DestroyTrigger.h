#pragma once

#include "Trigger.h"

class DestroyTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
	int bookCountV2 = 0;
protected:

	bool triggered = false;
};

