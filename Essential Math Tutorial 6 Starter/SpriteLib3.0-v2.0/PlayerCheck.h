#pragma once

#include "Trigger.h"
class PlayerCheck : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
protected:


};

