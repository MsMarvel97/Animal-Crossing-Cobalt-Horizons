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
	ECS::GetComponent<Checker>(m_targetEntities[0]).SetCollisionTracker(true);
	
}

void PosCheck::OnExit()
{
	Trigger::OnExit();
	ECS::GetComponent<Checker>(m_targetEntities[0]).SetCollisionTracker(false);
}


