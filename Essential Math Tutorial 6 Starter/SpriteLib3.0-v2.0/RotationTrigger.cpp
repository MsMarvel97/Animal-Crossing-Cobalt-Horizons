#include "RotationTrigger.h"
#include "ECS.h"

RotationTrigger::RotationTrigger()
{
}

void RotationTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).TriggeredRotation(rotation, true);
			triggered = true;
		}
	}
}

void RotationTrigger::OnExit()
{
}
