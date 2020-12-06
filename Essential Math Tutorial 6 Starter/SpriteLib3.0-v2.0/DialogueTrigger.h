#pragma once
#include "Trigger.h"

class DialogueTrigger : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;

};

