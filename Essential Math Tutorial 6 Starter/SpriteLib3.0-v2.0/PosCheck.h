#pragma once

#include "Trigger.h"
class PosCheck : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
	bool inZone = false;
	bool onEnter(bool& check);
protected:

	
};

