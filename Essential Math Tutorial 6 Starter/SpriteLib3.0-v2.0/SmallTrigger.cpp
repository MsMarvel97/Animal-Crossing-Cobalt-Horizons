#include "SmallTrigger.h"
#include "ECS.h"

void SmallTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void SmallTrigger::OnEnter()
{
	Trigger::OnEnter();

	std::cout << "small";
}

void SmallTrigger::OnExit()
{
	Trigger::OnExit();
}
