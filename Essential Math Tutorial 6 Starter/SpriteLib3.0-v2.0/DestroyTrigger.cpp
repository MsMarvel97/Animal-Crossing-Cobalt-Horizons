#include "DestroyTrigger.h"
#include "ECS.h"

void DestroyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
	
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
		}

		triggered = true;
	}
}

void DestroyTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
			if (PhysicsBody::m_bodiesToDelete.size() > 0)
			{
				std::cout << "It works!";
			}
			m_targetEntities.clear();
		}
		triggered = true;
	}
	
}

void DestroyTrigger::OnExit()
{
	Trigger::OnExit();	
}
