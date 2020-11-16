#include "PosCheck.h"
#include "ECS.h"

#include "Input.h"
void PosCheck::OnTrigger()
{
	Trigger::OnTrigger();
}

void PosCheck::OnEnter()
{
	Trigger::OnEnter();
	SetCheck(true);
	
}

void PosCheck::OnExit()
{
	Trigger::OnExit();
	SetCheck(false);
}


