#include "TranslateTrigger.h"


TranslateTrigger::TranslateTrigger()
{
}

void TranslateTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(movement, true);
			triggered = true;
		}
	}
}


void TranslateTrigger::OnExit()
{
}
