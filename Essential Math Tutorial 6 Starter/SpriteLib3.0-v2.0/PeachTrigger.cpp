#include "PeachTrigger.h"
#include "ECS.h"

void PeachTrigger::OnEnter()
{
	Trigger::OnEnter();

	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		if (ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer()).GetStun() == false)
		{
			ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer()).GainPeachPoint();
			ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer()).SetPointGain(true);
		}

		PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
	
		//if (PhysicsBody::m_bodiesToDelete.size() > 0)
		//{
		//	std::cout << "It works!";
		//}
		m_targetEntities.clear();
	}

}

void PeachTrigger::OnExit()
{
	Trigger::OnExit();
}
