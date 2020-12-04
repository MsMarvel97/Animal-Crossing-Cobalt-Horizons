#pragma once
#include "Trigger.h"

class RecheckConditions : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;

protected:
};
