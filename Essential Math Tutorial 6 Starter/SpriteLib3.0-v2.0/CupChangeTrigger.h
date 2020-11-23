#pragma once
#include "Trigger.h"
class CupChangeTrigger : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;
};

