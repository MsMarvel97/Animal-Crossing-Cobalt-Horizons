#include "RecheckConditions.h"
#include "ECS.h"

void RecheckConditions::OnEnter()
{
	Trigger::OnEnter();
	//for each entity
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		//check if target entity's collisionFlag component is true
		if (ECS::GetComponent<CollisionFlag>(m_targetEntities[i]).GetCollisionTracker() == true)
		{
			//add the set true target as a target of the targeted trigger
			ECS::GetComponent<Trigger*>(m_targetTrigger)->AddTargetEntity(m_trueTarget);

			//debug text
			if (m_targetEntities.size() > 0)
			{
				std::cout << "entity added" << std::endl;
			}
		}

	}
}

void RecheckConditions::OnExit()
{
	Trigger::OnExit();
	//clear targets of targeted trigger
	ECS::GetComponent<Trigger*>(m_targetTrigger)->ClearTargetEntities();
	//debug text
	if (ECS::GetComponent<Trigger*>(m_targetTrigger)->GetTargetEntities().size() == 0)
	{
		std::cout << "entities removed" << std::endl;

	}
}
