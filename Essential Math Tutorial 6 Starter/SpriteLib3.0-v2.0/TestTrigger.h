#pragma once
#include "Trigger.h"

class TestTrigger : public Trigger
{
public: 
	void OnEnter() override;
	void OnExit() override;
};

