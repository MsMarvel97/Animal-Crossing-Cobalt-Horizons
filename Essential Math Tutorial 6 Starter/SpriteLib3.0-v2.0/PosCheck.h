#pragma once
#include "Trigger.h"

class PosCheck : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;	
};

