#pragma once

#include "Trigger.h"

class CoconutTrigger : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;
};

