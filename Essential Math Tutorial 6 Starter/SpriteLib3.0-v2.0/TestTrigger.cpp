#include "TestTrigger.h"
#include "ECS.h"

void TestTrigger::OnEnter()
{
	Trigger::OnEnter();
	std::cout << "In\n";
}

void TestTrigger::OnExit()
{
	Trigger::OnExit();
	std::cout << "Out\n";
}